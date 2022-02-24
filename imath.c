#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>
#include <pthread.h>

#define THREADS 4

#define FILTER_WIDTH 3
#define FILTER_HEIGHT 3

#define RGB_MAX 255

typedef struct {
	 unsigned char r, g, b;
} PPMPixel;

struct parameter {
	PPMPixel *image;         //original image
	PPMPixel *result;        //filtered image
	unsigned long int w;     //width of image
	unsigned long int h;     //height of image
	unsigned long int start; //starting point of work
	unsigned long int size;  //equal share of work (almost equal if odd)
};


/*This is the thread function. It will compute the new values for the region of image specified in params (start to start+size) using convolution.
    (1) For each pixel in the input image, the filter is conceptually placed on top ofthe image with its origin lying on that pixel.
    (2) The  values  of  each  input  image  pixel  under  the  mask  are  multiplied  by the corresponding filter values.
    (3) The results are summed together to yield a single output value that is placed in the output image at the location of the pixel being processed on the input.
 
 */
void *threadfn(void *params)
{
	
	int laplacian[FILTER_WIDTH][FILTER_HEIGHT] =
	{
	  -1, -1, -1,
	  -1,  8, -1,
	  -1, -1, -1,
	};

    int red, green, blue;
    
    /*For all pixels in the work region of image (from start to start+size)
      Multiply every value of the filter with corresponding image pixel. Note: this is NOT matrix multiplication.
     
     //truncate values smaller than zero and larger than 255
      Store the new values of r,g,b in p->result.
     */
		
	return NULL;
}


/*Create a new P6 file to save the filtered image in. Write the header block
 e.g. P6
      Width Height
      Max color value
 then write the image data.
 The name of the new file shall be "name" (the second argument).
 */
void writeImage(PPMPixel *image, char *name, unsigned long int width, unsigned long int height)
{

    
}

/* Open the filename image for reading, and parse it.
    Example of a ppm header:    //http://netpbm.sourceforge.net/doc/ppm.html
    P6                  -- image format
    # comment           -- comment lines begin with
    ## another comment  -- any number of comment lines
    200 300             -- image width & height
    255                 -- max color value
 
 Check if the image format is P6. If not, print invalid format error message.
 Read the image size information and store them in width and height.
 Check the rgb component, if not 255, display error message.
 Return: pointer to PPMPixel that has the pixel data of the input image (filename)
 */
PPMPixel *readImage(const char *filename, unsigned long int *width, unsigned long int *height)
{

  PPMPixel *img;
  int check;
  int fileMaxRGB;
  char input;
  char string[2];
       
  FILE* fp;
    fp = fopen(filename, "r");
    
    if(fp == NULL)
    {
      printf("NULL ERROR\n");
	    exit(1);
    }

    printf("Opening file success\n");

    //read image format
    //check the image format by reading the first two characters in filename and compare them to P6
    for(int i = 0; i < 2; i++)
    {
      fread(&input, sizeof(char), 1, fp);
      string[i] = input;
    }
    if(strcmp(string, "P6") != 0)
    {
      printf("Error, file does not match compatible type\n");
      exit(1);
    }

      //If there are comments in the file, skip them. You may assume that comments exist only in the header block.

    check = fread(&input, sizeof(char), 1, fp);
    if(check == 0)
      printf("error first\n");
    
    check = fread(&input, sizeof(char), 1, fp);
    if(check == 0)
      printf("error second\n");

    char* tempbuf = (char*) malloc(sizeof(char) * 1024);
    
    while(input == '#'){
      //go to the next line
      fgets(tempbuf, 1024, fp);
      //check the next character

      
      if(fread(&input, sizeof(char), 1, fp) == 0){
	printf("ERROR\n");
	exit(1);
      }
    }
	//read image size information
    
    ungetc((int)input, fp);
    fscanf(fp, "%ld", width);
    fscanf(fp, "%ld", height);
    printf("%ld   %ld\n", *width, *height);

    //check for max byte
    //Read rgb component.  Check if it is equal to RGB_MAX. If not, display error message.
    fscanf(fp, "%d", &fileMaxRGB);
    printf("%d\n", fileMaxRGB);
    
    //allocate memory for img. NOTE: A ppm image of w=200 and h=300 will contain 60000 triplets (i.e. for r,g,b), ---> 18000 bytes.

    //read pixel data from filename into img. The pixel data is stored in scanline order from left to right (up to bottom) in 3-byte chunks (r g b values for each pixel) encoded as binary numbers.
   
    
    /* for(int i = 0; i < 900; i++){
      fread(&rgbValues, sizeof(char), 1, fp);
      printf("%c ", rgbValues);
    }
    printf("\n"); */

    int red, green, blue;

    for(int i = 0; i < (*width * *height); i++){
      red = fgetc(fp);
      green = fgetc(fp);
      blue = fgetc(fp);

      printf("%d %d %d\n", red, green, blue);
    }
  printf("%ld   %ld\n", *width, *height);
    return img;
}

/* Create threads and apply filter to image.
 Each thread shall do an equal share of the work, i.e. work=height/number of threads.
 Compute the elapsed time and store it in *elapsedTime (Read about gettimeofday).
 Return: result (filtered image)
 */
PPMPixel *apply_filters(PPMPixel *image, unsigned long w, unsigned long h, double *elapsedTime) {

    PPMPixel *result;
    //allocate memory for result

    //allocate memory for parameters (one for each thread)

    /*create threads and apply filter.
     For each thread, compute where to start its work.  Determine the size of the work. If the size is not even, the last thread shall take the rest of the work.
     */
   

   //Let threads wait till they all finish their work.


	return result;
}


/*The driver of the program. Check for the correct number of arguments. If wrong print the message: "Usage ./a.out filename"
    Read the image that is passed as an argument at runtime. Apply the filter. Print elapsed time in .3 precision (e.g. 0.006 s). Save the result image in a file called laplacian.ppm. Free allocated memory.
 */
int main(int argc, char *argv[])
{
  //load the image into the buffer

  /*
   * Things we need to get:
   * 1. "Magic" number (P6)
   * 2. Width and heigh of the ppm
   * 3. 
   */
    unsigned long int w, h;
    double elapsedTime = 0.0;
    
    if(argc < 2)
    {
      printf("Usage ./imath filename\n");
    }
    readImage(argv[1], &w, &h);

	return 0;
}
