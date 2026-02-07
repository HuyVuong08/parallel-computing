// Resources

//      https://pages.tacc.utexas.edu/~eijkhout/pcse/html/mpi-comm.html
//      https://www.boost.org/doc/libs/1_71_0/doc/html/mpi/tutorial.html#mpi.tutorial.communicators
//      https://mpitutorial.com/tutorials/introduction-to-groups-and-communicators/
//      http://pages.tacc.utexas.edu/~eijkhout/pcse/html/mpi-topo.html
//      http://www.rookiehpc.com

#include <mpi.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <random>

#include <sstream>
#include <iostream>

#include "CStopWatch.h"

#include <boost/mpi.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>
#include <boost/mpi/cartesian_communicator.hpp>
#include <boost/mpi/environment.hpp>
namespace mpi = boost::mpi;


// Split the communicator based on the color and use 
// the original rank for ordering
void example1(){

    int worldRank, worldNumProcs;
    MPI_Comm rowComm;
    int rowRank, rowSize;
    int color;

    MPI_Comm_rank(MPI_COMM_WORLD, &worldRank);
    MPI_Comm_size(MPI_COMM_WORLD, &worldNumProcs);

    color = worldRank/4;

    MPI_Comm_split(MPI_COMM_WORLD, color, worldRank, &rowComm);

    MPI_Comm_rank(rowComm, &rowRank);
    MPI_Comm_size(rowComm, &rowSize);

    std::cout << " World Rank/Size: " << worldRank << "/" << worldNumProcs;
    std::cout << "\t";
    std::cout << "Row Rank/Size: " << rowRank << "/" << rowSize;

    std::cout << "\n";

    MPI_Comm_free(&rowComm);

}

// Create a new communicator from prime ranked procs
// in World
void example2(){

    int worldRank, worldNumProcs;
    MPI_Comm_rank(MPI_COMM_WORLD, &worldRank);
    MPI_Comm_size(MPI_COMM_WORLD, &worldNumProcs);

    // Get the group of processes in the whole world
    MPI_Group worldGroup;
    MPI_Comm_group(MPI_COMM_WORLD, &worldGroup);

    int n = 4;
    const int ranks[4] = {2, 3, 5, 7};

    MPI_Group primeGroup;

    MPI_Group_incl(worldGroup, n, ranks, &primeGroup);

    MPI_Comm primeComm;
    MPI_Comm_create_group(MPI_COMM_WORLD, primeGroup, 0, &primeComm);

    int primeRank = -1, primeSize = -1;

    if(MPI_COMM_NULL != primeComm){
        MPI_Comm_rank(primeComm, &primeRank);
        MPI_Comm_size(primeComm, &primeSize);
    }

    std::cout << " World Rank/Size: " << worldRank << "/" << worldNumProcs;
    std::cout << "\t";
    std::cout << "Prime Rank/Size: " << primeRank << "/" << primeSize;

    std::cout << "\n";

    MPI_Group_free(&worldGroup);
    MPI_Group_free(&primeGroup);
    MPI_Comm_free(&primeComm);

}

// Split processes into generator and collector communicators randomly
void example3(){
   
    mpi::communicator world;
    bool isGenerator = world.rank() < 2 * world.size() / 3;
    mpi::communicator local = world.split(isGenerator ? 0 : 1);

    std::cout << "Local " << local.rank() << "/" << local.size();
    std::cout << " World " << world.rank() << "/" << world.size();
    
    if(isGenerator)  { std::cout << " generates.\n";}
    else             { std::cout << " collects.\n";}
}

// Cartesian Coordinator in Boost
void example4()
{
    mpi::communicator world;

    if (world.size() != 24) { return -1;}
  
    mpi::cartesian_dimension dims[] = {{2, true}, {3,true}, {4,true}};
    mpi::cartesian_communicator cart(world, mpi::cartesian_topology(dims));

    for (int r = 0; r < cart.size(); ++r) {
        cart.barrier();
        if (r == cart.rank()) {
            std::vector<int> c = cart.coordinates(r);
            std::cout << "rk :" << r << " coords: "<< c[0] << ' ' << c[1] << ' ' << c[2] << '\n';
        }
    }
  
  // Size of the default communicator
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
 
    // Ask MPI to decompose our processes in a 2D cartesian grid for us
    int dims[3] = {0, 0, 0};
    MPI_Dims_create(size, 3, dims);
 
    // Make both dimensions periodic
    int periods[3] = {true, true, true};
 
    // Let MPI assign arbitrary ranks if it deems it necessary
    int reorder = true;
 
    // Create a communicator given the 2D torus topology.
    MPI_Comm new_communicator;
    MPI_Cart_create(MPI_COMM_WORLD, 3, dims, periods, reorder, &new_communicator);
 
    // My rank in the new communicator
    int my_rank;
    MPI_Comm_rank(new_communicator, &my_rank);
 
    // Get my coordinates in the new communicator
    int my_coords[2];
    MPI_Cart_coords(new_communicator, my_rank, 2, my_coords);
 
    // Print my location in the 2D torus.
    printf("[MPI process %d] I am located at (%d, %d).\n", my_rank, my_coords[0],my_coords[1]);
}

int main(int argc, char **argv)
{
    mpi::environment env;

    example4();

    return 0;
}

/*
Communicator is a class that represents a group of processes that can communicate (e.g: sending, receiving, broadcasts and reductions) with each other. We can also create communicators using MPI_Comm_split and MPI_Comm_create_group functions.

Suppose comm_sz = 4 and suppose that x is a vector with n = 14 components.

a. How would the components of x be distributed among the processes in a program that used a block distribution?
b. How would the components of x be distributed among the processes in a program that used a cyclic distribution?
c. How would the components of x be distributed among the processes in a program that used a block-cyclic distribution with blocksize b = 2?

a. In a block distribution, the components of x would be divided by comm_sz (14/4 = 3 and 2 remainder). The 2 remainder will be distributed one by one to the first processes as below:
- Process 0: x[0] to x[3] (4 components)
- Process 1: x[4] to x[7] (4 components)
- Process 2: x[8] to x[10] (3 components)
- Process 3: x[11] to x[13] (3 components)

b. In a cyclic distribution, each process receives one element at a time, the assigning procedure is repeated until all elements are distributed as shown below: 
- Process 0: x[0], x[4], x[8], x[12]
- Process 1: x[1], x[5], x[9], x[13]
- Process 2: x[2], x[6], x[10]
- Process 3: x[3], x[7], x[11]

c. In a block-cyclic distribution with blocksize b = 2, each process receives b=2 elements at a time, the assigning procedure is repeated until all elements are distributed as shown below:
- Process 0: x[0],x[1], x[8],x[9]
- Process 1: x[2],x[3], x[10],x[11]
- Process 2: x[4],x[5], x[12],x[13]
- Process 3: x[6],x[7]
*/