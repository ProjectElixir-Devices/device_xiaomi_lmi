echo 'Cloning Kernel Tree'
# Kernel Tree
git clone --depth=1 https://github.com/ProjectElixir-Devices/kernel_xiaomi_lmi.git -b zen_plus-13 kernel/xiaomi/lmi

echo 'Cloning Vendor Tree'
# Vendor Tree
git clone --depth=1 https://github.com/ProjectElixir-Devices/vendor_xiaomi_lmi.git -b Tiramisu vendor/xiaomi/lmi

# Project Elixir Patch
echo 'Adding Patch to fix Brightness Slider Curve'
cd frameworks/base && wget https://raw.githubusercontent.com/sarthakroy2002/random-stuff/main/Patches/Fix-brightness-slider-curve-for-some-devices-a12l.patch && patch -p1 < *.patch && cd -
