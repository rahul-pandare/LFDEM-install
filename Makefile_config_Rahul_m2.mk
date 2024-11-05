#=======================================#
#    Parameters to provide              #
#=======================================#


# Use DSFMT instead of MT as a RNG ( yes / no )
DSFMT_RNG = no
# Enable use of Metis library ( yes / no )
UseMetis = no

# the directory where LF_DEM will be copied on `make install`
install_dir = /Users/Rahul/opt

# C++ compiler
# CXX = /opt/homebrew/Cellar/llvm@13/13.0.1_2/bin/clang++
# CC = /opt/homebrew/Cellar/llvm@13/13.0.1_2/bin/clang
CXX = /opt/homebrew/Cellar/llvm/19.1.0/bin/clang++
CC = /opt/homebrew/Cellar/llvm/19.1.0/bin/clang
#CXX = g++-14
#CC = gcc-14
#CXX = clang++
#CC = clang
#CXX = g++
#CC = gcc

# Libraries
#
# SuiteSparse library install folder
SUITESPARSE_ROOT = /opt/homebrew/Cellar/suite-sparse/7.8.2

# Extra flags to the compiler, if needed (e.g. optimization flags)
CXXFLAGS_EXTRA = -DGIT_VERSION="\"42ce875e-dirty\""

#======== Linking ==================

# Blas and Lapack (you may want to modify the BLAS, as -lblas might point to non optimized BLAS)
Blas_Linking_Flags = -lblas
Lapack_Linking_Flags = -L/opt/homebrew/opt/lapack/lib -llapack
# Intel MKL
# Blas_Linking_Flags = -mkl -lrt
# Lapack_Linking_Flags =

# Extra linking here, if needed
# Extra_Linking_Flags = /opt/homebrew/Cellar/suite-sparse/7.8.2/lib/libcamd.a /opt/homebrew/Cellar/suite-sparse/7.8.2/lib/libccolamd.a -fopenmp -Wl,-no_compact_unwind
Extra_Linking_Flags = -L/opt/homebrew/opt/llvm/lib/c++ -L/opt/homebrew/opt/llvm/lib -lunwind -fopenmp # -Wl,-no_compact_unwind
# for OpenMP with g++
# Extra_Linking_Flags = -fopenmp
