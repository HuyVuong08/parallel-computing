# Install script for directory: /users/PCS0294/hvuong/parallel-computing/cs4170_fa2025_project_g01/code/llvm-project/openmp/libomptarget/DeviceRTL

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/users/PCS0294/hvuong/local/libomp")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "0")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/users/PCS0294/hvuong/local/llvm-18.1.8/bin/llvm-objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "/users/PCS0294/hvuong/parallel-computing/cs4170_fa2025_project_g01/code/llvm-project/openmp/build-openmp/libomptarget/DeviceRTL/libomptarget-amdgpu-gfx700.bc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "/users/PCS0294/hvuong/parallel-computing/cs4170_fa2025_project_g01/code/llvm-project/openmp/build-openmp/libomptarget/DeviceRTL/libomptarget-amdgpu-gfx701.bc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "/users/PCS0294/hvuong/parallel-computing/cs4170_fa2025_project_g01/code/llvm-project/openmp/build-openmp/libomptarget/DeviceRTL/libomptarget-amdgpu-gfx801.bc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "/users/PCS0294/hvuong/parallel-computing/cs4170_fa2025_project_g01/code/llvm-project/openmp/build-openmp/libomptarget/DeviceRTL/libomptarget-amdgpu-gfx803.bc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "/users/PCS0294/hvuong/parallel-computing/cs4170_fa2025_project_g01/code/llvm-project/openmp/build-openmp/libomptarget/DeviceRTL/libomptarget-amdgpu-gfx900.bc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "/users/PCS0294/hvuong/parallel-computing/cs4170_fa2025_project_g01/code/llvm-project/openmp/build-openmp/libomptarget/DeviceRTL/libomptarget-amdgpu-gfx902.bc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "/users/PCS0294/hvuong/parallel-computing/cs4170_fa2025_project_g01/code/llvm-project/openmp/build-openmp/libomptarget/DeviceRTL/libomptarget-amdgpu-gfx906.bc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "/users/PCS0294/hvuong/parallel-computing/cs4170_fa2025_project_g01/code/llvm-project/openmp/build-openmp/libomptarget/DeviceRTL/libomptarget-amdgpu-gfx908.bc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "/users/PCS0294/hvuong/parallel-computing/cs4170_fa2025_project_g01/code/llvm-project/openmp/build-openmp/libomptarget/DeviceRTL/libomptarget-amdgpu-gfx90a.bc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "/users/PCS0294/hvuong/parallel-computing/cs4170_fa2025_project_g01/code/llvm-project/openmp/build-openmp/libomptarget/DeviceRTL/libomptarget-amdgpu-gfx90c.bc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "/users/PCS0294/hvuong/parallel-computing/cs4170_fa2025_project_g01/code/llvm-project/openmp/build-openmp/libomptarget/DeviceRTL/libomptarget-amdgpu-gfx940.bc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "/users/PCS0294/hvuong/parallel-computing/cs4170_fa2025_project_g01/code/llvm-project/openmp/build-openmp/libomptarget/DeviceRTL/libomptarget-amdgpu-gfx941.bc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "/users/PCS0294/hvuong/parallel-computing/cs4170_fa2025_project_g01/code/llvm-project/openmp/build-openmp/libomptarget/DeviceRTL/libomptarget-amdgpu-gfx942.bc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "/users/PCS0294/hvuong/parallel-computing/cs4170_fa2025_project_g01/code/llvm-project/openmp/build-openmp/libomptarget/DeviceRTL/libomptarget-amdgpu-gfx1010.bc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "/users/PCS0294/hvuong/parallel-computing/cs4170_fa2025_project_g01/code/llvm-project/openmp/build-openmp/libomptarget/DeviceRTL/libomptarget-amdgpu-gfx1030.bc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "/users/PCS0294/hvuong/parallel-computing/cs4170_fa2025_project_g01/code/llvm-project/openmp/build-openmp/libomptarget/DeviceRTL/libomptarget-amdgpu-gfx1031.bc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "/users/PCS0294/hvuong/parallel-computing/cs4170_fa2025_project_g01/code/llvm-project/openmp/build-openmp/libomptarget/DeviceRTL/libomptarget-amdgpu-gfx1032.bc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "/users/PCS0294/hvuong/parallel-computing/cs4170_fa2025_project_g01/code/llvm-project/openmp/build-openmp/libomptarget/DeviceRTL/libomptarget-amdgpu-gfx1033.bc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "/users/PCS0294/hvuong/parallel-computing/cs4170_fa2025_project_g01/code/llvm-project/openmp/build-openmp/libomptarget/DeviceRTL/libomptarget-amdgpu-gfx1034.bc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "/users/PCS0294/hvuong/parallel-computing/cs4170_fa2025_project_g01/code/llvm-project/openmp/build-openmp/libomptarget/DeviceRTL/libomptarget-amdgpu-gfx1035.bc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "/users/PCS0294/hvuong/parallel-computing/cs4170_fa2025_project_g01/code/llvm-project/openmp/build-openmp/libomptarget/DeviceRTL/libomptarget-amdgpu-gfx1036.bc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "/users/PCS0294/hvuong/parallel-computing/cs4170_fa2025_project_g01/code/llvm-project/openmp/build-openmp/libomptarget/DeviceRTL/libomptarget-amdgpu-gfx1100.bc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "/users/PCS0294/hvuong/parallel-computing/cs4170_fa2025_project_g01/code/llvm-project/openmp/build-openmp/libomptarget/DeviceRTL/libomptarget-amdgpu-gfx1101.bc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "/users/PCS0294/hvuong/parallel-computing/cs4170_fa2025_project_g01/code/llvm-project/openmp/build-openmp/libomptarget/DeviceRTL/libomptarget-amdgpu-gfx1102.bc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "/users/PCS0294/hvuong/parallel-computing/cs4170_fa2025_project_g01/code/llvm-project/openmp/build-openmp/libomptarget/DeviceRTL/libomptarget-amdgpu-gfx1103.bc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "/users/PCS0294/hvuong/parallel-computing/cs4170_fa2025_project_g01/code/llvm-project/openmp/build-openmp/libomptarget/DeviceRTL/libomptarget-amdgpu-gfx1150.bc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "/users/PCS0294/hvuong/parallel-computing/cs4170_fa2025_project_g01/code/llvm-project/openmp/build-openmp/libomptarget/DeviceRTL/libomptarget-amdgpu-gfx1151.bc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "/users/PCS0294/hvuong/parallel-computing/cs4170_fa2025_project_g01/code/llvm-project/openmp/build-openmp/libomptarget/DeviceRTL/libomptarget-nvptx-sm_35.bc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "/users/PCS0294/hvuong/parallel-computing/cs4170_fa2025_project_g01/code/llvm-project/openmp/build-openmp/libomptarget/DeviceRTL/libomptarget-nvptx-sm_37.bc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "/users/PCS0294/hvuong/parallel-computing/cs4170_fa2025_project_g01/code/llvm-project/openmp/build-openmp/libomptarget/DeviceRTL/libomptarget-nvptx-sm_50.bc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "/users/PCS0294/hvuong/parallel-computing/cs4170_fa2025_project_g01/code/llvm-project/openmp/build-openmp/libomptarget/DeviceRTL/libomptarget-nvptx-sm_52.bc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "/users/PCS0294/hvuong/parallel-computing/cs4170_fa2025_project_g01/code/llvm-project/openmp/build-openmp/libomptarget/DeviceRTL/libomptarget-nvptx-sm_53.bc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "/users/PCS0294/hvuong/parallel-computing/cs4170_fa2025_project_g01/code/llvm-project/openmp/build-openmp/libomptarget/DeviceRTL/libomptarget-nvptx-sm_60.bc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "/users/PCS0294/hvuong/parallel-computing/cs4170_fa2025_project_g01/code/llvm-project/openmp/build-openmp/libomptarget/DeviceRTL/libomptarget-nvptx-sm_61.bc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "/users/PCS0294/hvuong/parallel-computing/cs4170_fa2025_project_g01/code/llvm-project/openmp/build-openmp/libomptarget/DeviceRTL/libomptarget-nvptx-sm_62.bc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "/users/PCS0294/hvuong/parallel-computing/cs4170_fa2025_project_g01/code/llvm-project/openmp/build-openmp/libomptarget/DeviceRTL/libomptarget-nvptx-sm_70.bc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "/users/PCS0294/hvuong/parallel-computing/cs4170_fa2025_project_g01/code/llvm-project/openmp/build-openmp/libomptarget/DeviceRTL/libomptarget-nvptx-sm_72.bc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "/users/PCS0294/hvuong/parallel-computing/cs4170_fa2025_project_g01/code/llvm-project/openmp/build-openmp/libomptarget/DeviceRTL/libomptarget-nvptx-sm_75.bc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "/users/PCS0294/hvuong/parallel-computing/cs4170_fa2025_project_g01/code/llvm-project/openmp/build-openmp/libomptarget/DeviceRTL/libomptarget-nvptx-sm_80.bc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "/users/PCS0294/hvuong/parallel-computing/cs4170_fa2025_project_g01/code/llvm-project/openmp/build-openmp/libomptarget/DeviceRTL/libomptarget-nvptx-sm_86.bc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "/users/PCS0294/hvuong/parallel-computing/cs4170_fa2025_project_g01/code/llvm-project/openmp/build-openmp/libomptarget/DeviceRTL/libomptarget-nvptx-sm_87.bc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "/users/PCS0294/hvuong/parallel-computing/cs4170_fa2025_project_g01/code/llvm-project/openmp/build-openmp/libomptarget/DeviceRTL/libomptarget-nvptx-sm_89.bc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "/users/PCS0294/hvuong/parallel-computing/cs4170_fa2025_project_g01/code/llvm-project/openmp/build-openmp/libomptarget/DeviceRTL/libomptarget-nvptx-sm_90.bc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/users/PCS0294/hvuong/parallel-computing/cs4170_fa2025_project_g01/code/llvm-project/openmp/build-openmp/libomptarget/libomptarget.devicertl.a")
endif()

