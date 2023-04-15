echo 'Cloning Kernel Tree'
# Kernel Tree
git clone --depth=1 https://github.com/projects-nexus/nexus_kernel_xiaomi_sm8250 -b stable kernel/xiaomi/lmi


echo 'Cloning Vendor Tree'
# Vendor Tree
git clone --depth=1 https://github.com/ProjectElixir-Devices/vendor_xiaomi_lmi.git -b Tiramisu vendor/xiaomi/lmi

echo 'Clone Specific Clang'
# Clang
mkdir clang-r487747
wget -P clang-r487747 https://android.googlesource.com/platform/prebuilts/clang/host/linux-x86/+archive/refs/heads/master/clang-r487747.tgz
tar -xvf clang-r487747/clang-r487747.tgz
mv clang-r487747 prebuilts/clang/host/linux-x86/

echo 'Clone Kprofiles'
# Kprofile App
git clone --depth=1 https://github.com/KProfiles/android_packages_apps_Kprofiles -b main packages/apps/Kprofiles
