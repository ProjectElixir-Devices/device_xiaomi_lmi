/*
 * Copyright (C) 2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "Fingerprint.h"

#include <android-base/properties.h>
#include <fingerprint.sysprop.h>
#include "util/Util.h"

#include <android-base/logging.h>
#include <android-base/strings.h>

using namespace ::android::fingerprint::xiaomi;

namespace aidl::android::hardware::biometrics::fingerprint {

namespace {
constexpr int SENSOR_ID = 0;
constexpr common::SensorStrength SENSOR_STRENGTH = common::SensorStrength::STRONG;
constexpr int MAX_ENROLLMENTS_PER_USER = 5;
constexpr bool SUPPORTS_NAVIGATION_GESTURES = false;
constexpr char HW_COMPONENT_ID[] = "fingerprintSensor";
constexpr char HW_VERSION[] = "vendor/model/revision";
constexpr char FW_VERSION[] = "1.01";
constexpr char SERIAL_NUMBER[] = "00000001";
constexpr char SW_COMPONENT_ID[] = "matchingAlgorithm";
constexpr char SW_VERSION[] = "vendor/version/revision";
}  // namespace

static const uint16_t kVersion = HARDWARE_MODULE_API_VERSION(2, 1);
static Fingerprint* sInstance;

Fingerprint::Fingerprint() {
    sInstance = this;  // keep track of the most recent instance
    mSensorType = FingerprintSensorType::UNDER_DISPLAY_OPTICAL;
    mDevice = getSensorHal();
    std::thread([this]() {
        int fd = open(FOD_UI_PATH, O_RDONLY);
        if (fd < 0) {
            LOG(ERROR) << "failed to open fd, err: " << fd;
            return;
        }

        int fs = open(FOD_STATUS_PATH, O_RDWR);
        if (fs < 0) {
            LOG(ERROR) << "failed to open fs, err: " << fs;
        }

        struct pollfd fodUiPoll = {
            .fd = fd,
            .events = POLLERR | POLLPRI,
            .revents = 0,
        };

        while (true) {
            int rc = poll(&fodUiPoll, 1, -1);
            if (rc < 0) {
                LOG(ERROR) << "failed to poll fd, err: " << rc;
                continue;
            }

            bool fingerDown = readBool(fd);
            ALOGI("fod_ui status: %d", fingerDown);
            mDevice->extCmd(mDevice, COMMAND_NIT,
                            readBool(fd) ? PARAM_NIT_FOD : PARAM_NIT_NONE);
            if (!fingerDown) {
                set(DISPPARAM_PATH, DISPPARAM_FOD_HBM_OFF);
            }
            if (fs >= 0) {
                write(fs, readBool(fd) ? "1" : "0", 1);
            }
        }
    }).detach();
}

Fingerprint::~Fingerprint() {
    ALOGV("~Fingerprint()");
    if (mDevice == nullptr) {
        ALOGE("No valid device");
        return;
    }
    int err;
    if (0 != (err = mDevice->common.close(reinterpret_cast<hw_device_t*>(mDevice)))) {
        ALOGE("Can't close fingerprint module, error: %d", err);
        return;
    }
    mDevice = nullptr;
}

fingerprint_device_t* Fingerprint::openSensorHal(const char* class_name) {
    const hw_module_t* hw_mdl = nullptr;

    ALOGD("Opening fingerprint hal library...");
    if (hw_get_module_by_class(FINGERPRINT_HARDWARE_MODULE_ID, class_name, &hw_mdl) != 0) {
        ALOGE("Can't open fingerprint HW Module");
        return nullptr;
    }

    if (!hw_mdl) {
        ALOGE("No valid fingerprint module");
        return nullptr;
    }

    auto module = reinterpret_cast<const fingerprint_module_t*>(hw_mdl);
    if (!module->common.methods->open) {
        ALOGE("No valid open method");
        return nullptr;
    }

    hw_device_t* device = nullptr;
    if (module->common.methods->open(hw_mdl, nullptr, &device) != 0) {
        ALOGE("Can't open fingerprint methods");
        return nullptr;
    }

    auto fp_device = reinterpret_cast<fingerprint_device_t*>(device);
    if (fp_device->set_notify(fp_device, Fingerprint::notify) != 0) {
        ALOGE("Can't register fingerprint module callback");
        return nullptr;
    }

    return fp_device;
}

fingerprint_device_t* Fingerprint::getSensorHal() {
    if (mDevice) {
        ALOGI("fingerprint HAL already opened");
    } else {
        auto mod = FingerprintHalProperties::sensor_modules().value_or("");
        auto cla = ::android::base::Split(mod, ",");
        for (const std::string& class_name : cla) {
            mDevice = openSensorHal(class_name.c_str());
            if (!mDevice) {
                ALOGE("Can't open HAL module, class %s", class_name.c_str());
                continue;
            }
            ALOGI("Opened fingerprint HAL, class %s", class_name.c_str());
            break;
        }
        if (!mDevice) {
            ALOGE("Can't open any fingerprint HAL module");
        }
    }
    return mDevice;
}

SensorLocation Fingerprint::getSensorLocation() {
    SensorLocation location;

    auto loc = FingerprintHalProperties::sensor_location().value_or("");
    auto isValidStr = false;
    auto dim = ::android::base::Split(loc, "|");

    if (dim.size() != 3) {
        if (!loc.empty()) ALOGE("Invalid sensor location input (x|y|radius): %s", loc.c_str());
    } else {
        int32_t x, y, r;
        isValidStr = ParseInt(dim[0], &x) && ParseInt(dim[1], &y) && ParseInt(dim[2], &r);
        if (isValidStr) {
            location = {.sensorLocationX = x, .sensorLocationY = y, .sensorRadius = r};
        }
    }

    return location;
}

void Fingerprint::notify(const fingerprint_msg_t* msg) {
    Fingerprint* thisPtr = sInstance;
    if (thisPtr == nullptr || thisPtr->mSession == nullptr || thisPtr->mSession->isClosed()) {
        ALOGE("Receiving callbacks before a session is opened.");
        return;
    }
    thisPtr->mSession->notify(msg);
}

ndk::ScopedAStatus Fingerprint::getSensorProps(std::vector<SensorProps>* out) {
    std::vector<common::ComponentInfo> componentInfo = {
            {HW_COMPONENT_ID, HW_VERSION, FW_VERSION, SERIAL_NUMBER, "" /* softwareVersion */},
            {SW_COMPONENT_ID, "" /* hardwareVersion */, "" /* firmwareVersion */,
             "" /* serialNumber */, SW_VERSION}};
    common::CommonProps commonProps = {SENSOR_ID, SENSOR_STRENGTH,
                                       MAX_ENROLLMENTS_PER_USER, componentInfo};

    SensorLocation sensorLocation = getSensorLocation();

    ALOGI("sensor type: %s, location: %s", 
          ::android::internal::ToString(mSensorType).c_str(), 
          sensorLocation.toString().c_str());

    *out = {{commonProps,
             mSensorType,
             {sensorLocation},
             false, /* navigationGuesture */
             false, /* detectInteraction*/
             false, /* displayTouch*/
             true, /* controlIllumination*/
             std::nullopt}};
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Fingerprint::createSession(int32_t /*sensorId*/, int32_t userId,
                                              const std::shared_ptr<ISessionCallback>& cb,
                                              std::shared_ptr<ISession>* out) {
    CHECK(mSession == nullptr || mSession->isClosed()) << "Open session already exists!";

    mSession = SharedRefBase::make<Session>(getSensorHal(), userId, cb, mLockoutTracker);
    *out = mSession;

    mSession->linkToDeath(cb->asBinder().get());

    return ndk::ScopedAStatus::ok();
}

}  // namespace aidl::android::hardware::biometrics::fingerprint
