echo 'Cloning Kernel Tree'
# Kernel Tree
git clone --depth=1 https://github.com/projects-nexus/nexus_kernel_xiaomi_sm8250 -b stable kernel/xiaomi/lmi


echo 'Cloning Vendor Tree'
# Vendor Tree
git clone --depth=1 https://github.com/ProjectElixir-Devices/vendor_xiaomi_lmi.git -b Tiramisu vendor/xiaomi/lmi

echo 'Clone Kprofiles'
# Kprofile App
git clone --depth=1 https://github.com/KProfiles/android_packages_apps_Kprofiles -b main packages/apps/Kprofiles

echo 'Clone Prebuilt Apps Repo'
# Prebuilt Apps
git clone --depth=1 https://gitlab.com/ZenkaBestia/device_xiaomi_lmi_prebuilt-apps -b main device/xiaomi/lmi-prebuilt-apps
