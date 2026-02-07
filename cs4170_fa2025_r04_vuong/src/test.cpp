#include <mpi.h>
#include <sstream>
#include <iostream>
#include <iomanip>


struct trapData {
private:
    double a;
    double b;
    int n;

public:
    trapData() : a(0.0), b(0.0), n(0) {}
    trapData(double aa, double bb, int nn) : a(aa), b(bb), n(nn) {}

    // pointer accessors return const pointers
    const double* a_ptr() const { return &a; }
    const double* b_ptr() const { return &b; }
    const int*    n_ptr() const { return &n; }

    // getters / setters for use in code
    double getA() const { return a; }
    double getB() const { return b; }
    int    getN() const { return n; }

    void setA(double v) { a = v; }
    void setB(double v) { b = v; }
    void setN(int v) { n = v; }
};

double f(double x) {
   return x*x;
}

double Trap(double left, double right, int numTraps, double h) {
    
    double retValue, x; 

    retValue = (f(left) + f(right))/2.0;
    for(int i = 1; i <= numTraps-1; i++) {
        x = left + i*h;
        retValue += f(x);
     }
    retValue = retValue*h;

    return retValue;
} /*  Trap  */

void firstMethod(){
    int numProcs;   
    int myRank;    
    MPI_Status myStatus;
    double a, b, h, n, localA, localB, result, localResult;

    // MPI_Init(NULL, NULL); 
    MPI_Comm_size(MPI_COMM_WORLD, &numProcs); 
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank); 

    a = 0.0;
    b = 3.0;
    n = 1024;       // # of Trapezoids
    h = (b-a)/n;
    localA = a + myRank*(n/numProcs)*h;
    localB = localA + (n/numProcs)*h;
    localResult = Trap(localA, localB, n/numProcs, h);
    // std::cout << std::setw(5) << myRank << " " << localA <<  " " << localB << " " << localResult << "\n";

    if(myRank != 0) { 
        MPI_Send(&localResult, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD); 
    }else{
        result = localResult;
        for(int i = 1; i < numProcs; i++) {
            MPI_Recv(&localResult, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &myStatus);
            // std::cout << "Received " << localResult << " from " << i << "\n";
             
            result += localResult;
        }

        std::cout << "FirstMethod: The Result is: " << result << "\n";
    }
}

void secondMethod(){
    int numProcs;   
    int myRank;    
    double a, b, h, n, localA, localB, result, localResult;

    MPI_Comm_size(MPI_COMM_WORLD, &numProcs); 
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank); 

    a = 0.0;
    b = 3.0;
    n = 1024;       // # of Trapezoids
    h = (b-a)/n;
    localA = a + myRank*(n/numProcs)*h;
    localB = localA + (n/numProcs)*h;
    localResult = Trap(localA, localB, n/numProcs, h);

    // std::cout << myRank << " " << localA <<  " " << localB << " " << localResult << "\n";
    MPI_Reduce(&localResult, &result, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    if(myRank == 0) { 
        std::cout << "SecondMethod: The Result is: " << result << "\n";
    }
}

void thirdMethod(){
     int numProcs;   
    int myRank;    
    double a, b, h, n, localA, localB, result, localResult;

    MPI_Comm_size(MPI_COMM_WORLD, &numProcs); 
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank); 

    // Simulates getting input on Node 0
    if(myRank == 0) { 
        a = 0.0;
        b = 3.0;
        n = 1024;       // # of Trapezoids
        h = (b-a)/n;
    }
    MPI_Bcast(&a, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&b, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&n, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&h, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    localA = a + myRank*(n/numProcs)*h;
    localB = localA + (n/numProcs)*h;
    localResult = Trap(localA, localB, n/numProcs, h);

    // std::cout << myRank << " " << localA <<  " " << localB << " " << localResult << "\n";
    MPI_Reduce(&localResult, &result, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    if(myRank == 0) { 
        std::cout << "ThirdMethod: The Result is: " << result << "\n";
    }

    
}

void fourthMethod(){
    int numProcs;   
    int myRank;    
    double h, localA, localB, result, localResult;

    /********* NEW *********/
    trapData t;
    MPI_Datatype trapDataType;

    int blockLengths[3] = {1};
    MPI_Datatype mpiDataTypes[3] = {MPI_DOUBLE, MPI_DOUBLE, MPI_INT};
    MPI_Aint displacements[3] = {0};
    MPI_Aint aAddr, bAddr, nAddr;

    t.setA(0.0);
    t.setB(3.0);
    t.setN(1024);

    MPI_Get_address(t.a_ptr(), &aAddr);
    MPI_Get_address(t.b_ptr(), &bAddr);
    MPI_Get_address(t.n_ptr(), &nAddr);
    displacements[1] = bAddr - aAddr;
    displacements[2] = nAddr - aAddr;

    MPI_Type_create_struct(3, blockLengths, displacements, mpiDataTypes, &trapDataType);
    MPI_Type_commit(&trapDataType);

    /***********************/

    MPI_Comm_size(MPI_COMM_WORLD, &numProcs); 
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank); 

    // Simulates getting input on Node 0
    if(myRank == 0) { 
        // a = 0.0;
        // b = 3.0;
        // n = 1024;       // # of Trapezoids
        // h = (b-a)/n;
        t.setA(0.0);
        t.setB(3.0);
        t.setN(1024);
    }

    /********* NEW *********/
    MPI_Bcast(&t, 1, trapDataType, 0, MPI_COMM_WORLD);

    h = (t.getB()-t.getA())/t.getN();
    localA = t.getA() + myRank*(t.getN()/numProcs)*h;
    localB = localA + (t.getN()/numProcs)*h;
    localResult = Trap(localA, localB, t.getN()/numProcs, h);

    /***********************/

    // std::cout << std::setw(5) << myRank << " " << localA <<  " " << localB << " " << localResult << "\n";
    MPI_Reduce(&localResult, &result, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    if(myRank == 0) { 
        std::cout << "FourthMethod: The Result is: " << result << "\n";
    }

    /********* NEW *********/
    MPI_Type_free(&trapDataType);
    /************************/
}
int main(){

    MPI_Init(NULL, NULL); 

    thirdMethod();
    fourthMethod();
 
    MPI_Finalize(); 

    return 0;
}