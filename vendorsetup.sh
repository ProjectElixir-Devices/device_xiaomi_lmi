rm -rf kernel/xiaomi/
git clone --depth=1 https://github.com/ProjectElixir-Devices/kernel_xiaomi_lmi.git -b Kitsune kernel/xiaomi/lmi

rm -rf vendor/xiaomi
git clone --depth=1 https://github.com/ProjectElixir-Devices/vendor_xiaomi_lmi.git -b vany vendor/xiaomi/lmi

rm -rf device/xiaomi/lmi-prebuilt-apps
git clone --depth=1 https://gitlab.com/ZenkaBestia/device_xiaomi_lmi_prebuilt-apps -b main device/xiaomi/lmi-prebuilt-apps

rm -rf vendor/xiaomi/lmi-miuicamera
git clone --depth=1 https://gitlab.com/ZenkaBestia/vendor_xiaomi_lmi-miuicamera.git -b 15.0 vendor/xiaomi/lmi-miuicamera
