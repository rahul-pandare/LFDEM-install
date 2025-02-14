#=======================================#
#    Parameters to provide              #
#=======================================#


# Use DSFMT instead of MT as a RNG ( yes / no )
DSFMT_RNG = no
# Enable use of Metis library ( yes / no )
UseMetis = yes

# the directory where LF_DEM will be copied on `make install`
install_dir = /Users/rahul/opt

# C++ compiler
CXX = g++
CC = gcc

# Libraries
#
# SuiteSparse library install folder
SUITESPARSE_ROOT = /opt/homebrew/Cellar/suite-sparse/7.0.1

# Extra flags to the compiler, if needed (e.g. optimization flags)
CXXFLAGS_EXTRA = -I /opt/homebrew/include -DGIT_VERSION="\"42ce875e-dirty\""


#======== Linking ==================

# Blas and Lapack (you may want to modify the BLAS, as -lblas might point to non optimized BLAS)
Blas_Linking_Flags = -lblas
Lapack_Linking_Flags = -llapack
# Intel MKL
# Blas_Linking_Flags = -mkl -lrt
# Lapack_Linking_Flags =

# Extra linking here, if needed
Extra_Linking_Flags = /opt/homebrew/lib/libcamd.a /opt/homebrew/lib/libccolamd.a
# for OpenMP with g++
# Extra_Linking_Flags = -fopenmp
