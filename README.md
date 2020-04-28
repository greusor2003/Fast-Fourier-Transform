# Fast-Fourier-Transform

This is a C++ STL implementation of the classic radix -2 FFT algorithm.
It works with complex type input arrays of dimension a power of 2. A std::vector<double> can be fed to the transform as well 
via its data() method provided that its size is determined at compile time. The header file contains void fft<size_t>(cx*) as a 
simple fast Fourier transform and fftp as a parallelised slightly faster version. The inverse transform is implemented as 
void ifft<size_t>(cx*). A driver program for testining can be built out of main.cpp using the makefile.  
