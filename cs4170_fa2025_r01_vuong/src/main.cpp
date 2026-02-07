#include <mpi.h>
#include <sstream>
#include <iostream>


int main(){

    std::string msg;
    int  numProcs;   
    int  myRank;    
    MPI_Status myStatus;

    MPI_Init(NULL, NULL); 

    MPI_Comm_size(MPI_COMM_WORLD, &numProcs); 

    MPI_Comm_rank(MPI_COMM_WORLD, &myRank); 
    if(myRank != 0) { 
        std::ostringstream out; 
        out << "Greetings from " << myRank << " of " << numProcs;
        msg = out.str();
        MPI_Send(msg.c_str(), msg.size()+1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
   } else {  
    
        int count; 
        char* buf;
        
        for (int q = 1; q < numProcs; q++) {

            MPI_Probe(q, 0, MPI_COMM_WORLD, &myStatus);
            MPI_Get_count(&myStatus, MPI_CHAR, &count);

            buf = new char[count];
            MPI_Recv(buf, count, MPI_CHAR, q, 0, MPI_COMM_WORLD, &myStatus);

            std::cout << buf << "\n";      
      } 
    }

    MPI_Finalize(); 

    return 0;
}