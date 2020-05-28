/********************
**  Color Palette generator
** Skeleton by Justin Yokota
**********************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include "ColorMapInput.h"
#include <sys/stat.h>
//You don't need to call this function but it helps you understand how the arguments are passed in 
void usage(char* argv[])
{
	printf("Incorrect usage: Expected arguments are %s <inputfile> <outputfolder> <width> <heightpercolor>", argv[0]);
}

//Creates a color palette image for the given colorfile in outputfile. Width and heightpercolor dictates the dimensions of each color. Output should be in P3 format;
int P3colorpalette(char* colorfile, int width, int heightpercolor, char* outputfile)
{// initalizing things
    int lengthOfFile;
    uint8_t** colorarray; 
    int  colorcount1;
    int i;
    int j;
    int ver= 1;
    int w = 1;
    int counter;
    int vertical;
    uint8_t * liner;
// create the pointer to the files
	FILE* outputer = fopen(outputfile, "w+");
// make sure evevrtyhing is valid
  if(outputer == NULL){
		//exit(1);
		printf("ohno\n");
	}
// get the size of the file 
        colorarray = FileToColorMap(colorfile, &colorcount1);
	vertical = heightpercolor * colorcount1;
// write it to file should be the first line
        fprintf(outputer,"P3 %d %d %d\n", width, vertical, 255);
	for(i=0 ; i < colorcount1;i++){
 	uint8_t * pix  = colorarray[i];
	w = 0;
	ver = 0;
	j = 0;
	counter = 0;
	int not;
        liner = (uint8_t*) malloc(sizeof(uint8_t)*3*width);
	while(w < width){
	for (j = 0; j<3;j++){
		liner[counter] = pix[j];  			
		counter ++;
		//printf(a "%d ", pix[j]);
		}
	w++;
	}
	while(ver < heightpercolor){
		   for(not=0 ; not< width-1; not++){
		fprintf(outputer,"%d %d %d ", liner[0], liner[1], liner[2]);
	}
       	fprintf(outputer,"%d %d %d\n", liner[0], liner[1], liner[2]);
	ver++;
	}
}
free(liner);	
// iterate through the array and make sure the width and height is correct; 	
int freeing;
for(freeing = 0; freeing < colorcount1; freeing++){
	free(colorarray[freeing]);}
free(colorarray);
fclose(outputer);

return 0;
}

//Same as above, but with P6 format
int P6colorpalette(char* colorfile, int width, int heightpercolor, char* outputfile)
{
 int lengthOfFile;
    uint8_t** colorarray;
    int  colorcount1 ;
    int i;
    int j;
    int ver= 1;
    int w = 1;
    int input;
    int counter;
    int vertical;
    
    uint8_t * liner;
// create the pointer to the files
        FILE* outputer2 = fopen(outputfile, "w+");
// make sure evevrtyhing is valid
  if(outputer2 == NULL){
                //exit(1);
                printf("ohno\n");
        }
// get the size of the file 
        colorarray = FileToColorMap(colorfile, &colorcount1);
        vertical = heightpercolor * colorcount1;
// write it to file should be the first line
        fprintf(outputer2,"P6 %d %d %d\n", width, vertical, 255);
        for(i=0 ; i < colorcount1;i++){
        w = 0;
        ver = 0;
        j = 0;
        counter = 0;
        int not; 
	uint8_t * pix  = colorarray[i];
	while (ver < heightpercolor){
        	while(w < width){
        		for (j = 0; j<3;j++){ 
	       			input = pix[j];
               			fwrite(&input,1,sizeof(char),outputer2);
			}
			w++;
			}
	w = 0;	
	ver++;
	}
}
for(i = 0; i< colorcount1; i++){
	free(colorarray[i]);
}
free(colorarray);
fclose(outputer2);
return 0;
}


//The one piece of c code you don't have to read or understand. Still, might as well read it, if you have time.
int main(int argc, char* argv[])
{
	if (argc != 5)
	{
		usage(argv);
		return 1;
	}
	int width = atoi(argv[3]);
	int height = atoi(argv[4]);
	char* P3end = "/colorpaletteP3.ppm";
	char* P6end = "/colorpaletteP6.ppm";
	char buffer[strlen(argv[2]) + strlen(P3end)+1];
	sprintf(buffer, "%s%s", argv[2], P3end);
	int failed = P3colorpalette(argv[1], width, height, buffer);
	if (failed)
	{
		printf("Error in making P3colorpalette");
		return 1;
	}
	sprintf(buffer, "%s%s", argv[2], P6end);
	failed = P6colorpalette(argv[1], width, height, buffer);
	if (failed)
	{
		printf("Error in making P6colorpalette");
		return 1;
	}
	printf("P3colorpalette and P6colorpalette ran without error, output should be stored in %s%s, %s%s", argv[2], P3end, argv[2], P6end);
	return 0;
}


