/*********************
**  Color Map generator
** Skeleton by Justin Yokota
**********************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include "ColorMapInput.h"


/**************
**This function reads in a file name colorfile.
**It then uses the information in colorfile to create a color array, with each color represented by an int[3].
***************/
uint8_t** FileToColorMap(char* colorfile, int* colorcount)
{ 
// make the file ptr and check
FILE* ptr = fopen(colorfile, "r");
   if (ptr == NULL){
     return 0;
   }
// make a place to store the number of pixels and variable to index into a 2D array
  int numberofpix;
  int counter = 0, i = 0, j= 0;
   //if (fscanf(ptr,"%1d", numberofpix) != 1){
     //exit(1);
	
//need to read the first line of the file and set it to color count
   rewind(ptr);
   fscanf(ptr,"%d",&numberofpix);
   
   printf("numbe of  pix : %d\n ",numberofpix);
   *colorcount = numberofpix; 
// make and alloc the 2D array 
   uint8_t ** arr = (uint8_t ** )malloc(numberofpix * sizeof(uint8_t*));
   for (i=0 ; i <= numberofpix; i++){
     arr[i] = (uint8_t *)malloc(3 * sizeof(uint8_t));
   }
/// get second line and start adding them to an array number of pix x 3
  // int *numberfromfile =(int *) malloc(sizeof(int)*numberofpix * 3);
  int *numberfromfile =(int *) malloc(sizeof(int)*numberofpix * 3);
   int number;
   i=0;
   do{fscanf(ptr,"%d", &number); //Storing the number into the array
        numberfromfile[i] = number ;
    	i ++;
    } while(!feof(ptr));
   for(i=0; i < numberofpix; i++){
     for (j =0 ; j <3 ; j++){
        if (numberfromfile[counter] < 0 || numberfromfile[counter] > 255 ){
	printf("%d",numberfromfile[counter]);
        printf("i think we got here\n"); 
       }
       arr[i][j] = numberfromfile[counter];

       counter ++;
     }
   }
  // close file and return the array
    fclose(ptr);
    free(numberfromfile);
    return arr;
}


