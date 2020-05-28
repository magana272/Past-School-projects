/*********************
**  Mandelbrot fractal
** clang -Xpreprocessor -fopenmp -lomp -o Mandelbrot Mandelbrot.c
** by Dan Garcia <ddgarcia@cs.berkeley.edu>
** Modified for this class by Justin Yokota and Chenyu Shi
**********************/

#include <stdio.h>
#include <stdlib.h>
#include "ComplexNumber.h"
#include "Mandelbrot.h"
#include <sys/types.h>

/*
This function returns the number of iterations before the initial point >= the threshold.
If the threshold is not exceeded after maxiters, the function returns 0.
*/
u_int64_t MandelbrotIterations(u_int64_t maxiters, ComplexNumber * point, double threshold)
{
u_int64_t iterations = 0;
  ComplexNumber * temp = point;
  ComplexNumber * Z = newComplexNumber(0,0);
  ComplexNumber * M;
  ComplexNumber *temptwo;
  while(ComplexAbs(Z) < threshold){
    temptwo  = ComplexProduct(Z, Z);
    freeComplexNumber(Z);
    M  = ComplexSum(temptwo, temp);
    freeComplexNumber(temptwo); 
    Z  = M;
    iterations ++;
    if(iterations >= maxiters){
      freeComplexNumber(Z);
      return 0;
    }
  }
  freeComplexNumber(M);
  return iterations;
}

/*
This function calculates the Mandelbrot plot and stores the result in output.
The number of pixels in the image is resolution * 2 + 1 in one row/column. It's a square image.
Scale is the the distance between center and the top pixel in one dimension.
*/
void Mandelbrot(double threshold, u_int64_t max_iterations, ComplexNumber* center, double scale, u_int64_t resolution, u_int64_t * output){
     u_int64_t numOfpixels = (2 * resolution + 1);
     double centerreal = Re(center);
     double centerim = Im(center);
     ComplexNumber * hori;
     ComplexNumber * vertical;
     ComplexNumber * left = newComplexNumber(centerreal - scale, centerim + scale);
     ComplexNumber * down;
     ComplexNumber * put = newComplexNumber(0,0);
     int counter = 0;
     int i;
     int j;
     for(int i = 0 ; i < numOfpixels; i++){
	freeComplexNumber(put);	
	down = newComplexNumber(0 , -i*scale / resolution);
	put  = ComplexSum(left, down);
        output[counter] = MandelbrotIterations(max_iterations, put, threshold);
	freeComplexNumber(down);
        counter++;
       for (int j = 1; j < numOfpixels; j++){
	 vertical = newComplexNumber(j * scale / resolution, 0 );
	 hori = ComplexSum(put, vertical);
    	 output[counter] = MandelbrotIterations(max_iterations, hori, threshold);
         counter++;
	 freeComplexNumber(hori);
    	 freeComplexNumber(vertical);				}
}
	freeComplexNumber(left);
	freeComplexNumber(put);
}
