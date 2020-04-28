#include <iostream>
#include <cmath>
#include <complex>
#include <bitset>
#include <omp.h>


using cx = std::complex<double>;

constexpr double PI = 3.141592653589793;


// C style vector length deduction
template < class T, size_t n>
constexpr size_t len(T(&)[n]) noexcept
{
    return n;
}

// reverses the order of bits
template <size_t N>
constexpr void bit_rev(std::bitset<N>& b)
{
    for (size_t i = 0; i < N/2; i++ )
    {
        bool x = b.test(i);
        b.set(i,b.test(N-i-1));
        b.set(N-i-1, x);
    }
 }

// rearranges the vector v by reversing the bit order of the indexes bit representations
template < class T, size_t n>
void fft_ready(T* v)
{
    constexpr size_t N = static_cast<size_t>(log2(n));
    std::bitset<N> b;
    for (size_t i = 0; i < n; i++)
    {
        b=i;
        bit_rev(b); size_t j = b.to_ullong();
        if (j > i)
            std::swap (v[i], v[j]);
    }
}

// input = A = results of two fft of length n/2; output = A = fft of length n.
void step(cx* A, size_t n, int sign)
{
    auto w = cx(cos(-2*sign*PI/(double)n), sin(-2*sign*PI/(double)n));
    cx q = 1; size_t j;

        for (j = 0; j < n/2; j++)
        {
            auto t = A[j]; auto u = q*A[j+n/2];
            A[j] = t + u;
            A[j+n/2] = t - u;
            q = q*w;
        }    
}

// array A of length n is transformed using log2(n) many step(A, n, sign) routines
 
void radix2(cx* A, size_t n, int sign)
{
    auto N = static_cast<size_t>(log2(n));

    size_t m =2; 
    for (size_t s = 1; s <= N; s++)
    {
        for (size_t k = 0; k < n; k = k + m)
        {
            step(A+k, m, sign);
        }
            m *= 2;
    }
}

//parallel version of radix2
void radix2p(cx* A, size_t n, int sign)
{
    #pragma omp parallel num_threads(4)
    {
        #pragma omp sections
        {
            #pragma omp section  
            radix2(A, n/4, sign);

            #pragma omp section
            radix2(A+n/4, n/4, sign);

            #pragma omp section  
            radix2(A+n/2, n/4, sign);

            #pragma omp section  
            radix2(A+3*n/4, n/4, sign);
        }
        
        #pragma omp sections
        {
            #pragma omp section
                step(A, n/2, sign);

            #pragma omp section
                step(A+n/2, n/2, sign);
        }
    } 
        
        step(A, n, sign);
    
}

// fft for A vector of length n = 2^N
template <size_t n>
void fft(cx* A)
{
    fft_ready<cx, n>(A);
    
    radix2(A, n, 1);    
}

// fft parallel version
template < size_t n >
void fftp(cx* A)
{
    fft_ready<cx, n>(A);
    
    radix2p(A, n, 1);   
}

// inverse fftp 
template <size_t n>
void ifftp(cx* A)
{
    fft_ready<cx, n>(A);

    radix2p(A, n, -1);   
    
    for (size_t k = 0; k < n; k++)
    {
        A[k] = A[k] / static_cast<double>(n);
    }
        
}
