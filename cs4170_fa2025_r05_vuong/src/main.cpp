#include <omp.h>
#include <iostream>
#include "CStopWatch.h"

double f(double x){
    double retVal = 0.0;

    retVal = x*x;

    return retVal;
}

double serialTrap(double a, double b, int n){

    double h, retValue, x;

    h = (b-a)/n;
    retValue = (f(a) + f(b))/2.0;

    #pragma omp parallel for reduction(+:retValue)
    for(int i=0; i<n; i++){
        x = a + (i+1)*h;
        retValue += f(x);
    }
    retValue = h*retValue;

    return retValue;
}


int main(){

    int threadMin, threadMax;
    int numTrials;
    int a, b, n;
    CStopWatch timer;
    double result;

    a = 0, b = 16, n = 500000000;
    threadMin = 1; threadMax = 12;
    numTrials = 10;

    std::cout << "Threads, Result, Time\n";

    for(int numThreads=threadMin; numThreads<=threadMax; numThreads++){
        for(int curTrial=0; curTrial<numTrials; curTrial++){
            
            omp_set_num_threads(numThreads);
            
            result = 0.0;
            timer.startTimer();
            result = serialTrap(a, b, n);
            timer.stopTimer();

            std::cout << numThreads << ", " << result << ", " << timer.getElapsedTime() << "\n";
        }
    }
    
    return 0;
}
