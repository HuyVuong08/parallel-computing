#!/bin/bash

# Load required modules
module load cuda/12.6.2
module load gcc/12.3.0

# Add custom CMake and local LLVM to PATH
export PATH=/users/PCS0294/hvuong/local/llvm-18.1.8/bin:/users/PCS0294/hvuong/local/cmake-3.28.3/bin:$PATH

# fast delete ./build dir due to IO limitations on shared remote login nodes
#mkdir -p ./build
#mkdir -p ./emptydir
#rsync -a --delete ./emptydir ./build
#rm -rf ./emptydir

rm -rf ./build 
mkdir -p ./build

cd ./build

# We have designed the CMakeLists.txt for building HeCBench with clang/clang++, as we use
# clang-specific flags. We originally had it working with NVCC, but for simplification
# purposes we stick to clang. You may be able to get it working with NVCC, just will
# need to adjust some build flags for the compilation (which we've left commented
# in the CMakeLists.txt file)

# If you're having issues building, it's most likely due to include order issues.
# Add `-H` to the build flags to see what include files are being added and 
# in what order the include directories get searched at build time.
# For example, you can use `make target-name 2>&1 | grep -ni "math.h"` to find the instances
# of the math header being included and decide if clang is including the correct one

# You may need to adjust the order of included directories like we do for Lassen
# below. Adding the `-nobuiltininc` gets rid of a lot of auto system directories
# that clang tries to smartly add. Then you can go in an add the directories yourself
# as we do below.  

# for some reason on lassen, clang is struggling to properly order the include
# directories at build time, so we need to forcibly set the correct directories
LASSEN_OMP_FLAGS="-isystem /usr/tce/packages/clang/clang-18.1.8/release/lib/clang/18/include -isystem /usr/tce/packages/clang/clang-18.1.8/release/lib/clang/18/include/openmp_wrappers -isystem /usr/tce/packages/gcc/gcc-11.2.1/rh/usr/include/c++/11 -isystem /usr/tce/packages/clang/clang-18.1.8/release/lib/clang/18/include/cuda_wrappers -nobuiltininc"

LASSEN_CUDA_FLAGS="-isystem /usr/tce/packages/clang/clang-18.1.8/release/lib/clang/18/include/cuda_wrappers -isystem /usr/tce/packages/gcc/gcc-11.2.1/rh/usr/include/c++/11 -isystem /usr/tce/packages/gcc/gcc-11.2.1/rh/usr/include/c++/11/ppc64le-redhat-linux -isystem /usr/tce/packages/cuda/cuda-12.2.2/nvidia/targets/ppc64le-linux/include -isystem /usr/tce/packages/gcc/gcc-11.2.1/rh/usr/include/c++/11/backward -isystem /usr/tce/packages/clang/clang-18.1.8/release/lib/clang/18/include -isystem /usr/tce/packages/cuda/cuda-12.2.2/include -nobuiltininc"

EXTRA_OMP_FLAGS=""
# Add libomp include paths so omp.h is found (BC passed via CMakeLists)
EXTRA_OMP_FLAGS="-I/users/PCS0294/hvuong/local/libomp/include -I/users/PCS0294/hvuong/local/libomp/include/openmp_wrappers"
# Ensure CUDA compilations see libomp headers as well (device BC provided in CMakeLists)
EXTRA_CUDA_FLAGS="-Xcompiler -fPIC -I/users/PCS0294/hvuong/local/libomp/include -I/users/PCS0294/hvuong/local/libomp/include/openmp_wrappers"

#EXTRA_BUILD_FLAGS="-O3 -v -H" 
#EXTRA_LINK_FLAGS="-v"

EXTRA_BUILD_FLAGS="-O3" 
EXTRA_LINK_FLAGS=""

# We have modified all the flags in the build system to be clang-specific
# We originally had this working with `nvcc` for the CUDA codes, but switched
# to LLVM because it's popular and keeps the build pipeline simpler. 
# It'll also allow us to build SYCL in the future.

# The CUDAToolkit_ROOT can be left as it, Cmake will auto-detect the correct one

# Make locally-built libomp visible to CMake's OpenMP detection
export LD_LIBRARY_PATH=/users/PCS0294/hvuong/local/libomp/lib:$LD_LIBRARY_PATH
export CPPFLAGS="-I/users/PCS0294/hvuong/local/libomp/include ${CPPFLAGS:-}"
export LDFLAGS="-L/users/PCS0294/hvuong/local/libomp/lib ${LDFLAGS:-}"
export CMAKE_PREFIX_PATH=/users/PCS0294/hvuong/local/libomp:${CMAKE_PREFIX_PATH:-}

export CC=$(which clang)
export CXX=$(which clang++)

# Make nvcc use clang++ as the host compiler so host-compiler flags
# (for example OpenMP flags like -fopenmp=libomp) are accepted.
export CUDAHOSTCXX=$(which clang++)

cmake -DCMAKE_C_COMPILER=$CC \
      -DCMAKE_CXX_COMPILER=$CXX \
      -DCMAKE_CUDA_COMPILER=$(which nvcc) \
      -DBUILD_ALL=ON \
      -DBUILD_OMP=ON \
      -DBUILD_CUDA=ON \
      -DCUDAToolkit_ROOT=/usr/local/cuda-12.6 \
      -DOpenMP_C_FLAGS='-fopenmp -I/users/PCS0294/hvuong/local/libomp/include' \
      -DOpenMP_C_LIB_NAMES='omp' \
      -DOpenMP_C_LIBRARIES='/users/PCS0294/hvuong/local/libomp/lib/libomp.so' \
      -DOpenMP_CXX_FLAGS='-fopenmp -I/users/PCS0294/hvuong/local/libomp/include' \
      -DOpenMP_CXX_LIB_NAMES='omp' \
      -DOpenMP_CXX_LIBRARIES='/users/PCS0294/hvuong/local/libomp/lib/libomp.so' \
      -DOpenMP_omp_LIBRARY='/users/PCS0294/hvuong/local/libomp/lib/libomp.so' \
      -DOpenMP_C_FOUND=TRUE \
      -DCMAKE_C_FLAGS="${EXTRA_BUILD_FLAGS}" \
      -DCMAKE_CXX_FLAGS="${EXTRA_BUILD_FLAGS}" \
      -DCMAKE_CUDA_FLAGS="${EXTRA_BUILD_FLAGS}" \
      -DCMAKE_BUILD_TYPE=Release \
      -DCUSTOM_OMP_FLAGS="${EXTRA_OMP_FLAGS}" \
      -DCUSTOM_CUDA_FLAGS="${EXTRA_CUDA_FLAGS}" \
      -DCUSTOM_OMP_LINK_FLAGS="${EXTRA_LINK_FLAGS}" \
      -DCUSTOM_CUDA_LINK_FLAGS="${EXTRA_LINK_FLAGS}" \
      -DCMAKE_CUDA_ARCHITECTURES="70" \
      -DCUDA_ARCH="70" \
      -S../ -B./

make -j20 all
