mkdir -p build
cd build
echo "Building on $(nproc) cores"
echo "Available RAM: $(free -m | awk 'NR==2{printf "%.2fGB", $7/1024}')"
cmake ..
make -j$(nproc)
