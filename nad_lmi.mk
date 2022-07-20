#
# Copyright (C) 2021 The LineageOS Project
#
# SPDX-License-Identifier: Apache-2.0
#

# Inherit from those products. Most specific first.
$(call inherit-product, $(SRC_TARGET_DIR)/product/core_64_bit.mk)
$(call inherit-product, $(SRC_TARGET_DIR)/product/full_base_telephony.mk)

# Inherit from lmi device
$(call inherit-product, device/xiaomi/lmi/device.mk)

# Inherit some common Nusantara Project stuff.
$(call inherit-product, vendor/nusantara/config/common_full_phone.mk)

# Inherit some common Nusantara Project stuff.
NAD_BUILD_TYPE := OFFICIAL
USE_GAPPS ?= true
USE_AOSP_CLOCK := true
USE_PIXEL_CHARGING := true
TARGET_USES_BLUR := true
TARGET_FOD_ANIMATIONS := true
TARGET_BOOT_ANIMATION_RES := 1080
TARGET_SUPPORTS_GOOGLE_RECORDER := true
TARGET_SUPPORTS_QUICK_TAP := true
EXTRA_FOD_ANIMATIONS := true

# Device identifier. This must come after all inclusions.
PRODUCT_NAME := nad_lmi
PRODUCT_DEVICE := lmi
PRODUCT_BRAND := POCO
PRODUCT_MODEL := POCO F2 Pro
PRODUCT_MANUFACTURER := Xiaomi

PRODUCT_CHARACTERISTICS := nosdcard

PRODUCT_GMS_CLIENTID_BASE := android-xiaomi

PRODUCT_BUILD_PROP_OVERRIDES += \
    PRIVATE_BUILD_DESC="lmi-user 12 RKQ1.211001.001 V13.0.4.0.SJKCNXM release-keys"

BUILD_FINGERPRINT := Redmi/lmi/lmi:12/RKQ1.211001.001/V13.0.4.0.SJKCNXM:user/release-keys
