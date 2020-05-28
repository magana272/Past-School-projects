/********************
**  Mandelbrot fractal movie generator
** clang -Xpreprocessor -fopenmp -lomp -o Mandelbrot Mandelbrot.c
** by Dan Garcia <ddgarcia@cs.berkeley.edu>
** Modified for this class by Justin Yokota and Chenyu Shi
**********************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "ComplexNumber.h"
#include "Mandelbrot.h"
#include "ColorMapInput.h"
#include <sys/types.h>

void printUsage(char* argv[])
{
  printf("Usage: %s <threshold> <maxiterations> <center_real> <center_imaginary> <initialscale> <finalscale> <framecount> <resolution>9 <output_folder> 10 <colorfile>\n", argv[0]);
  printf("    This program simulates the Mandelbrot Fractal, and creates an iteration map of the given center, scale, and resolution, then saves it in output_file\n");
}


/*
This function calculates the threshold values of every spot on a sequence of frames. The center stays the same throughout the zoom. First frame is at initialscale, and last frame is at finalscale scale.
The remaining frames form a geometric sequence of scales, so 
if initialscale=1024, finalscale=1, framecount=11, then your frames will have scales of 1024, 512, 256, 128, 64, 32, 16, 8, 4, 2, 1.
As another example, if initialscale=10, finalscale=0.01, framecount=5, then your frames will have scale 10, 10 * (0.01/10)^(1/4), 10 * (0.01/10)^(2/4), 10 * (0.01/10)^(3/4), 0.01 .
*/
void MandelMovie(double threshold, u_int64_t max_iterations, ComplexNumber* center, double initialscale, double finalscale, int framecount, u_int64_t resolution, u_int64_t ** output){
    int in;    
    for(in = 0; in < framecount; in++){
	double scale = initialscale*(double) pow((finalscale/initialscale), ((double)in/((double)framecount-1)));
    //   printf("%f",initialscale * pow((finalscale/initialscale), (in/framecount)));
       Mandelbrot(threshold, max_iterations, center,scale, resolution, output[in]);
}
}

/**************
**This main function converts command line inputs into the format needed to run MandelMovie.
**It then uses the color array from FileToColorMap to create PPM images for each frame, and stores it in output_folder
***************/

int main(int argc, char* argv[])
{
	//Tips on how to get started on main function: 
	//MandelFrame also follows a similar sequence of steps; it may be useful to reference that.
	//Mayke you complete the steps below in order. 

	//STEP 1: Convert command line inputs to local variables, and ensure that inputs are valid.
	/*
	Check the spec for examples of invalid inputs.
	Remember to use your solution to B.1.1 to process colorfile.
	*/

	//YOUR CODE HERE
	if (argc != 11){
                printf("%s: Wrong number of arguments, expecting 10\n", argv[0]);
                printUsage(argv);
                return 1;
        }
	double threshold1, initalscale1, finalscale1;
	int framecount1;
	uint8_t ** colorarray1;
	char * colorfile1;
	char *  output_folder;
        ComplexNumber* center2;
        u_int64_t max_iterations, resolution;
        threshold1 = atof(argv[1]);
        max_iterations = (u_int64_t)atoi(argv[2]);
        center2 = newComplexNumber(atof(argv[3]), atof(argv[4]));
        initalscale1 = atof(argv[5]);
	finalscale1 = atof(argv[6]);
	framecount1 = atof(argv[7]);
        resolution = (u_int64_t)atoi(argv[8]);
	output_folder = argv[9];
	colorfile1 = argv[10];
	//printf("%c\n",*colorfile1);
	int count;
	u_int64_t sizetwo = 2 * resolution + 1;
//	int *  count =(int*)malloc(sizeof(int));
	int i;
	int j;
	int k;
	int z;
	int inputter;
	uint64_t * holder;
	int counter = 00000;	
	uint8_t ** colormap1;
	char filename[1000];
	int input; 
	uint8_t * pix;
	int value;
	
	//int squ = resolution * 2 + 1;
       //1
       if (threshold1 <= 0 || initalscale1 <= 0 || max_iterations <= 0||framecount1>10000||framecount1<0 ) {
 	freeComplexNumber(center2);                
	return 1;
        }
	if(framecount1==1 && initalscale1 !=finalscale1){
		return 1;
	} 

	colormap1 = FileToColorMap(colorfile1, &count);
	//STEP 2: Run MandealMovie on the correct arguments.
	/*
	MandelMovie requires an output array, so make sure you allocate the proper amount of space. 
	If allocation fails, free all the space you have already allocated (including colormap), then return with exit code 1.
	*/
	uint64_t ** ar1 = (uint64_t**) malloc(framecount1*sizeof(uint64_t *));
	if(ar1 == NULL){
	freeComplexNumber(center2);
	return 1;}
	for (i=0 ; i < framecount1; i++){
     		ar1[i] = (uint64_t*)malloc((resolution * 2 + 1)*(resolution * 2 + 1)*sizeof(uint64_t));
		if(ar1[i] == NULL){
			freeComplexNumber(center2);
			for(j=0; j<=i;j++){
     		 		for(i = 0; i < count; i++){
        			free(colormap1[i]);
        				}
			free(ar1[j]);
		}
		free(ar1);
		return 1;}
	}
	MandelMovie(threshold1, max_iterations, center2 , initalscale1 , finalscale1, framecount1, resolution, ar1); 
	//STEP 3: Output the results of MandelMovie to .ppm files.
	/*
	Convert from iteration count to colors, and output the results into output files.
	Use what we showed you in Part B.1.2, create a seqeunce of ppm files in the output folder.
	Feel free to create your own helper function to complete this step.
	As a reminder, we are using P6 format, not P3.
	*/
	for(i= 0; i < framecount1; i++){
	holder = ar1[i];
	sprintf(filename,"%s\/frame%05d.ppm",output_folder,counter);
	printf("%s", filename);
	counter = counter + 1 ;
	FILE * newfile = fopen(filename, "w+");
	fprintf(newfile,"P6 %lu %lu %d\n",resolution * 2 + 1 , resolution * 2 + 1, 255);
		for(j =0 ; j<(resolution *2+1)*(resolution * 2 + 1);j++){
		 	if (holder[j] == 0){
				for(z = 0; z<3 ;z++){
					input = 0;
					//printf("%d",input);
					fwrite(&input,1,1,newfile);
					}
		 	}
			else{
		 	value =(((int)holder[j])-1) %(int) count;
			printf("%d", value);
			pix = colormap1[value]; 
			for(k=0;k<3;k++){
				inputter = pix[k]; 
				fwrite(&inputter,1,1,newfile);}
		 
		 

			     }
					}
}
	//STEP 4: Free all allocated memory
	/*
	Make sure there's no memory leak.
	*/
      freeComplexNumber(center2);
      for(i = 0; i < count; i++){
	free(colormap1[i]);
	}
      free(colormap1);
      for(i=0; i<framecount1;i++){
		 free(ar1[i]);}
	free(ar1);
	return 0;}
