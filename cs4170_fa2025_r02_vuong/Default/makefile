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


# All Target
all: MPI

OSC: MPI
# Tool invocations
MPI: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	$(CC) -o $(TARGET) $(ARCH) $(OBJS) $(LIBS) 
	@echo 'Finished building target: $@'
	@echo ' '

src/%.o: ../src/%.cpp
	@mkdir -p $(@D)
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(CC) $(INCLUDE) $(CFLAGS) $(LPSOLVE_FLAGS) $(OPENMP) $(ARCH) $(LIBS) -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

# Add a target to build and run pi_seq
PI_SEQ_SRC := ../src/pi_seq.cpp ../src/CStopWatch.cpp
PI_SEQ_BIN := pi_seq

pi_seq: $(PI_SEQ_SRC)
	$(CC) $(INCLUDE) $(CFLAGS) -o $(PI_SEQ_BIN) $(PI_SEQ_SRC) $(LIBS)

run_pi_seq: pi_seq
	./$(PI_SEQ_BIN)

PI_MPI_SRC := ../src/pi_mpi.cpp ../src/CStopWatch.cpp
PI_MPI_BIN := pi_mpi

pi_mpi: $(PI_MPI_SRC)
	$(CC) $(INCLUDE) $(CFLAGS) -o $(PI_MPI_BIN) $(PI_MPI_SRC) $(LIBS)

# build_Targets
clean:
	-$(RM) $(OBJS)$(CPP_DEPS)$(CPP_DEPS) $(TARGET)
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:
