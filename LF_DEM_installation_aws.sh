#!/bin/bash
# Script to install LF_DEM


sudo apt install zip
sudo apt install unzip
sudo apt update && sudo apt install g++ -y

unzip SuiteSparse-5.4.0.zip -d SuiteSparse
unzip lfdem.zip -d lfdem

# Update package list and upgrade existing packages
echo "Updating package list..."
sudo apt update -y
sudo apt upgrade -y

# Install GCC
echo "Installing GCC..."
sudo apt install -y gcc

# Install CMake
echo "Installing CMake..."
sudo apt install -y cmake

# Install OpenBLAS
echo "Installing OpenBLAS..."
sudo apt install -y libopenblas-dev

# Install LAPACK
echo "Installing LAPACK..."
sudo apt install -y liblapack-dev

# Verify installation
echo "Verifying installations..."
gcc --version
cmake --version
ldconfig -p | grep openblas
ldconfig -p | grep lapack

mkdir opt
mkdir simulations

sudo chmod -R 755 .

rm -r SuiteSparse-5.4.0.zip lfdem.zip

# installing suitesparse and lfdem
cd SuiteSparse; sudo make config; sudo make; sudo make install

cd lfdem/LF_DEM; sudo make; sudo make install

echo "LF_DEM Installation complete!"