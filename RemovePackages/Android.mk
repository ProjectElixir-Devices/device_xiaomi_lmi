LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := RemovePackages
LOCAL_MODULE_CLASS := APPS
LOCAL_MODULE_TAGS := optional
LOCAL_OVERRIDES_PACKAGES += Chrome Chrome-Stub Drive Maps PrebuiltGmail Videos YouTube YouTubeMusicPrebuilt
LOCAL_OVERRIDES_PACKAGES := Email Exchange Exchange2 UnifiedEmail PhotoTable Eleven
LOCAL_OVERRIDES_PACKAGES += PlayAutoInstallConfig SafetyHubPrebuilt RecorderPrebuilt
LOCAL_OVERRIDES_PACKAGES += TipsPrebuilt Aperture
LOCAL_UNINSTALLABLE_MODULE := true
LOCAL_CERTIFICATE := PRESIGNED
LOCAL_SRC_FILES := /dev/null
include $(BUILD_PREBUILT)
