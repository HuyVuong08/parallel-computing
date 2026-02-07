# Build serial and MPI binaries for matrix-vector multiplication
# NOTE: Make sure to load Boost module before running: module load boost/1.83.0

CXX     ?= c++
MPICXX  ?= mpicxx
CXXFLAGS = -std=c++17 -O3 -Wall -Wextra

# Boost libraries for MPI support
ifdef BOOST_ROOT
	BOOST_LDFLAGS = -L$(BOOST_ROOT)/lib
	BOOST_CXXFLAGS = -I$(BOOST_ROOT)/include
endif
BOOST_LIBS = $(BOOST_LDFLAGS) -lboost_mpi -lboost_serialization

SRCDIR  := ../src
BINDIR  := .

.PHONY: all serial mpi OSC run-serial run-mpi clean

all: serial mpi

serial: $(SRCDIR)/serials.cpp
	$(CXX) $(CXXFLAGS) -o $(BINDIR)/serials $(SRCDIR)/serials.cpp

mpi: $(SRCDIR)/parallel.cpp
	$(MPICXX) $(CXXFLAGS) $(BOOST_CXXFLAGS) -o $(BINDIR)/MPI $(SRCDIR)/parallel.cpp $(BOOST_LIBS)

# For OSC scripts expecting 'make OSC'
OSC: mpi

run-serial: serial
	./serials $(ARGS)

run-mpi: mpi
	mpiexec -n $(P) ./MPI $(ARGS)

clean:
	rm -f $(BINDIR)/serials $(BINDIR)/MPI