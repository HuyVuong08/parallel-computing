# Build serial and MPI binaries for matrix-vector multiplication
# NOTE: Make sure to load Boost module before running: module load boost/1.83.0

ifeq ($(origin CXX), default)
ifneq ($(shell command -v g++-15 >/dev/null 2>&1 && echo yes),)
CXX := g++-15
else
CXX := c++
endif
endif
MPICXX  ?= mpicxx
OPENCXX := $(notdir $(firstword $(CXX)))
CXXFLAGS = -std=c++17 -O3 -Wall -Wextra
OPENMP_FLAGS ?=
ifneq (,$(findstring g++,$(OPENCXX)))
OPENMP_FLAGS += -fopenmp
endif

# Boost libraries for MPI support
ifdef BOOST_ROOT
	BOOST_LDFLAGS = -L$(BOOST_ROOT)/lib
	BOOST_CXXFLAGS = -I$(BOOST_ROOT)/include
endif
BOOST_LIBS = $(BOOST_LDFLAGS) -lboost_mpi -lboost_serialization

SRCDIR  := ../src
BINDIR  := .

.PHONY: all serial mpi OSC run-serial run-mpi clean

SERIAL_SRC := $(SRCDIR)/matrix-multiplication.cpp
SERIAL_BIN := $(BINDIR)/matrix-multiplication
MPI_SRC := $(SRCDIR)/parallel.cpp
MPI_BIN := $(BINDIR)/MPI
TARGETS := serial
ifneq ($(wildcard $(MPI_SRC)),)
TARGETS += mpi
endif

all: $(TARGETS)

serial: $(SERIAL_SRC)
	$(CXX) $(CXXFLAGS) $(OPENMP_FLAGS) -o $(SERIAL_BIN) $(SERIAL_SRC)

ifeq ($(wildcard $(MPI_SRC)),)
mpi:
	$(error Missing MPI source '$(MPI_SRC)'. Add the file or update the makefile.)
else
mpi: $(MPI_SRC)
	$(MPICXX) $(CXXFLAGS) $(OPENMP_FLAGS) $(BOOST_CXXFLAGS) -o $(MPI_BIN) $(MPI_SRC) $(BOOST_LIBS)
endif

# For OSC scripts expecting 'make OSC'
OSC: mpi

run-serial: serial
	$(SERIAL_BIN) $(ARGS)

run-mpi: mpi
	mpiexec -n $(P) $(MPI_BIN) $(ARGS)

clean:
	rm -f $(SERIAL_BIN) $(MPI_BIN)
