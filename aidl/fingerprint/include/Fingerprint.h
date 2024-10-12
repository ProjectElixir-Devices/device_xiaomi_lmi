/*
 * Copyright (C) 2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <aidl/android/hardware/biometrics/fingerprint/BnFingerprint.h>
#include <android-base/properties.h>

#include <poll.h>
#include <unistd.h>
#include <fstream>
#include <thread>

#include "LockoutTracker.h"
#include "Session.h"
#include "fingerprint.h"

#define COMMAND_NIT 10
#define PARAM_NIT_FOD 1
#define PARAM_NIT_NONE 0

#define DISPPARAM_PATH "/sys/devices/platform/soc/ae00000.qcom,mdss_mdp/drm/card0/card0-DSI-1/disp_param"
#define DISPPARAM_FOD_HBM_OFF "0xE0000"

#define FOD_UI_PATH "/sys/devices/platform/soc/soc:qcom,dsi-display-primary/fod_ui"

#define FOD_STATUS_PATH "/sys/touchpanel/fod_status"

namespace {

template <typename T>
static void set(const std::string& path, const T& value) {
    std::ofstream file(path);
    file << value;
}

static bool readBool(int fd) {
    char c;
    int rc;

    rc = lseek(fd, 0, SEEK_SET);
    if (rc) {
        ALOGE("failed to seek fd, err: %d", rc);
        return false;
    }

    rc = read(fd, &c, sizeof(char));
    if (rc != 1) {
        ALOGE("failed to read bool from fd, err: %d", rc);
        return false;
    }

    return c != '0';
}

} // anonymous namespace

using ::aidl::android::hardware::biometrics::fingerprint::ISession;
using ::aidl::android::hardware::biometrics::fingerprint::ISessionCallback;
using ::aidl::android::hardware::biometrics::fingerprint::SensorProps;
using ::aidl::android::hardware::biometrics::fingerprint::FingerprintSensorType;

namespace aidl::android::hardware::biometrics::fingerprint {

class Fingerprint : public BnFingerprint {
public:
    Fingerprint();
    ~Fingerprint();

    ndk::ScopedAStatus getSensorProps(std::vector<SensorProps>* _aidl_return) override;
    ndk::ScopedAStatus createSession(int32_t sensorId, int32_t userId,
                                     const std::shared_ptr<ISessionCallback>& cb,
                                     std::shared_ptr<ISession>* out) override;

private:
    static fingerprint_device_t* openSensorHal(const char* class_name);
    fingerprint_device_t* getSensorHal();
    static SensorLocation getSensorLocation();
    static void notify(const fingerprint_msg_t* msg);

    std::shared_ptr<Session> mSession;
    LockoutTracker mLockoutTracker;
    FingerprintSensorType mSensorType;

    fingerprint_device_t* mDevice;
};

}  // namespace aidl::android::hardware::biometrics::fingerprint
