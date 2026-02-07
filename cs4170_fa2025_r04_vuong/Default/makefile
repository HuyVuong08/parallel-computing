OSC: LIBS = $(BOOST_LIBS) $(MPI_LIBS) -lgomp -lmpi -lboost_mpi -lboost_serialization
OSC: CFLAGS =-O2 -g -Wall -c -fmessage-length=0 -std=c++17 
OSC: INCLUDE = -I../src $(MPI_CXXFLAGS) $(BOOST_CFLAGS)

all: LIBS = $(BOOST_LIBS) -lgomp -lmpi -lboost_mpi -lboost_serialization
all: CFLAGS =-O2 -g -Wall -c -fmessage-length=0 -std=c++17
all: INCLUDE = -I../src  -I$(BOOST_CFLAGS)/

RM := rm -rf

CC = mpicxx 

TARGET  = "MPI"

UNAME := $(shell uname)
ifeq ($(UNAME),Darwin)
	INCLUDE = -I../src -I/usr/local/opt/boost/
else
	INCLUDE = -I../src -I/boost_1_80_0
	#-I$(MPI_CXXFLAGS) -I$(BOOST_FLAGS)
endif

SUBDIRS  	 := $(wildcard ../) $(wildcard ../*/)
CPP_SRCS 	 := $(wildcard ../*/*.cpp) $(wildcard ../*/*/*.cpp) 

OBJS     := $(patsubst ../%.cpp, ./%.o, $(CPP_SRCS))
CPP_DEPS := $(patsubst ../%.cpp, ./%.d, $(CPP_SRCS))

# Objects for MPI executable (do NOT include test.o)
MPI_OBJS := ./src/main.o ./src/CStopWatch.o
TEST_OBJS := ./src/test.o

BOOST_LIBS = -lboost_mpi -lboost_serialization
BOOST_LDFLAGS = -L/apps/spack/0.21/pitzer/linux-rhel9-skylake/boost/intel/2021.10.0/mvapich/3.0/1.83.0-ckaqrmu/lib

# compile ../src/*.cpp -> ./src/*.o
./src/%.o: ../src/%.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

# All Target
all: MPI

OSC: MPI
# Tool invocations

# Link MPI executable using MPI wrapper (CC)
MPI: $(MPI_OBJS)
	$(CC) -o MPI $(MPI_OBJS) $(BOOST_LDFLAGS) $(BOOST_LIBS) $(LDFLAGS) $(LDLIBS)

test: $(TEST_OBJS)
	$(CXX) -o test $(TEST_OBJS) $(LDFLAGS) $(LDLIBS)

# Other Targets
clean:
	$(RM) ./src/*.o ./src/*.d MPI test

.PHONY: all clean dependents
.SECONDARY:
