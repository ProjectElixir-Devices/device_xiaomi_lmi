echo 'Cloning Kernel Tree'
# Kernel Tree
git clone --depth=1 https://github.com/projects-nexus/nexus_kernel_xiaomi_sm8250 -b stable kernel/xiaomi/lmi


echo 'Cloning Vendor Tree'
# Vendor Tree
git clone --depth=1 https://github.com/ProjectElixir-Devices/vendor_xiaomi_lmi.git -b Tiramisu vendor/xiaomi/lmi
