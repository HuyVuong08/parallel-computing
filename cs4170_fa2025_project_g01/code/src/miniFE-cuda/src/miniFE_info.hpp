#ifndef miniFE_info_hpp
#define miniFE_info_hpp

#define MINIFE_HOSTNAME "p0254.ten.osc.edu"
#define MINIFE_KERNEL_NAME "'Linux'"
#define MINIFE_KERNEL_RELEASE "'5.14.0-427.96.1.el9_4.x86_64'"
#define MINIFE_PROCESSOR "'x86_64'"

#define MINIFE_CXX "'/apps/spack/0.21/pitzer/linux-rhel9-skylake/cuda/gcc/11.4.1/12.6.2-danbfon/bin/nvcc'"
#define MINIFE_CXX_VERSION "'nvcc: NVIDIA (R) Cuda compiler driver'"
#define MINIFE_CXXFLAGS "'-I. -I../utils -I../fem -DMINIFE_SCALAR=double -DMINIFE_LOCAL_ORDINAL=int -DMINIFE_GLOBAL_ORDINAL=int -DMINIFE_RESTRICT=__restrict__ -O3 -x cu -arch=sm_60 -DMINIFE_CSR_MATRIX '"

#endif
