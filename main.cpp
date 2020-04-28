#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include "fft.h"

using std::cout;
using std::cin;
using vdbl = std::vector<cx>;

//signal length
constexpr size_t n = 16;

template < class T>
void printv(T* v, size_t n) 
{
    for (size_t i = 0; i < n; i++)
        std::cout << *(v+i) << " ";
    std::cout << "\n"; 
}

int main()
{
    
    std::cout<< std::setprecision(16);
    vdbl v(n,0);
    for (size_t i = 0; i< n; i++)
        v[i] = cx((double)(i+1));   

    auto w(v);
/*
    double vt[n];
    for (size_t i = 0; i < n; i++ )
        vt[i] = i+1;
    

    cx cvt[len(vt)];
    for (size_t i = 0; i < len(vt); i++)
        cvt[i] = cx(vt[i]);
*/    
    
    auto start = std::chrono::system_clock::now(); //start timing
        
    //parallel fft on std::vector v, v.size() = N
	fftp<n>(v.data());
    
    auto end = std::chrono::system_clock::now(); //end of timing
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "\n Elapsed time: " << elapsed_seconds.count() << "s\n\n";
    
    
    printv<cx>(v.data(),n);
    
    std::cout << v[n-2] << "\n";
    
    //inverse fft on std::vector v, v.size() = N
    //ifftp<N>(v.data());
    
    //printv<cx>(v.data(), N);

   
  
    //std::cout << v[N-2] << "\n";

    
   
    
    return 0;
}
