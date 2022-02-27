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
  
  printf("You are here\n");
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
  FILE *fp;
  fp = fopen(name, "wb");
  printf("%ld %ld \n", width, height);

  //Writing the header block

  //P6
  fwrite("P6\n", sizeof(char), 3, fp);

  //Width Height
  fwrite("800", sizeof(char), 3, fp);
  fwrite(" ", sizeof(char), 1, fp);
  fwrite("600", sizeof(char), 3, fp);

  //Max color value
  fwrite("\n255\n", sizeof(char), 4, fp);

  fclose(fp);

  //fwrite(image, sizeof(PPMPixel), 1, fp);
  
  //add the values by row
  
  int pixelCounter;
  size_t check;
  int counter = 0;

  
  for(int i = 0; i < (height * width); i++)
  {
    counter++;
    if(pixelCounter == width)
    {
      //if the pixelcounter adds the width, then we go to the next line of the ppm
      fwrite("\n", sizeof(char), 1, fp);
      pixelCounter = 0;
    }
    printf("%d %d %d %d\n", image[i].r, image[i].g, image[i].b, counter);
    
    //check = fwrite(&image[i].r, sizeof(char), 1, fp);
    //fwrite(" ", sizeof(char), 1, fp);
    //check = fwrite(&image[i].g, sizeof(char), 1, fp);
    //fwrite(" ", sizeof(char), 1, fp);
    //check = fwrite(&image[i].b, sizeof(char), 1, fp);
    //fwrite(" ", sizeof(char), 1, fp);

    check = fwrite(image, sizeof(PPMPixel), 1, fp);
    //printf("%d %d %d\n", image->r, image->g, image->b);
    image++;

    if(check == 0){
      //error
      printf("Error in writeImage\n");
      //fclose(fp);
      //exit(1);
    }

    pixelCounter++;
  }
  
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
(PPMPixel*)malloc(sizeof(PPMPixel) * (*height) * (*width));
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
    
  while(input == '#')
  {
    //go to the next line
    fgets(tempbuf, 1024, fp);
    //check the next character
    if(fread(&input, sizeof(char), 1, fp) == 0)
    {
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
  //Read rgb component.  Check if it is equal toman pthread RGB_MAX. If not, display error message.
  fscanf(fp, "%d", &fileMaxRGB);
  printf("%d\n", fileMaxRGB);
    
  //allocate memory for img. NOTE: A ppm image of w=200 and h=300 will contain 60000 triplets (i.e. for r,g,b), ---> 18000 bytes.

  //read pixel data from filename into img. The pixel data is stored in scanline order from left to right (up to bottom) in 3-byte chunks (r g b values for each pixel) encoded as binary numbers.
   
    
    /* for(int i = 0; i < 900; i++){
      fread(&rgbValues, sizeof(char), 1, fp);
      printf("%c ", rgbValues);
    }
    printf("\n"); */

  PPMPixel* pix;
  pix = (PPMPixel*)malloc(sizeof(PPMPixel) * (*height) * (*width));

  //Takes the rgb values of each pixel and stores them into the array pix
  for(int i = 0; i < ((*width) * (*height)); i++)
  {
    pix[i].r = fgetc(fp);
    pix[i].g = fgetc(fp);
    pix[i].b = fgetc(fp);
  }

  //Print statement to check if values are printed
  int counter = 0;
  for(int i = 0; i < ((*width) * (*height)); i++)
  {
    counter++;
    printf("%d %d %d %d\n", pix[i].r, pix[i].g, pix[i].b, counter);
  }
  
  fclose(fp);
  return pix;
}

/* Create threads and apply filter to image.
 Each thread shall do an equal share of the work, i.e. work=height/number of threads.
 Compute the elapsed time and store it in *elapsedTime (Read about gettimeofday).
 Return: result (filtered image)
 */
PPMPixel *apply_filters(PPMPixel *image, unsigned long w, unsigned long h, double *elapsedTime) {

  PPMPixel *result;
  pthread_t aThread;
  void *tretA = 0;

  //allocate memory for result
  result = (PPMPixel*)malloc(sizeof(PPMPixel) * h * w);
  struct parameter a;
  a.image = image;
  a.result = result;
  a.w = w;
  a.h = h;
  a.start = 0;
  a.size = w * h;

  if(pthread_create(&aThread, NULL, threadfn, (void*)&a) != 0)
  {
    printf("ERROR CREATING THREAD\n");
    exit(1);
  }
  else
  {
    pthread_join(aThread, &tretA);
    printf("Thread created\n");
  }
  
  printf("Width: %ld\n", a.w);
  printf("Height: %ld\n", a.h);
  printf("Start: %ld\n", a.start);
  printf("Size: %ld\n", a.size);

  //allocate memory for parameters (one for each thread)
  //How many threads do we use?

  /*create threads and apply filter.
   For each thread, compute where to start its work.  Determine the size of the work. If the size is not even, the last thread shall take the rest of the work.
   */
  
  int size = (w * h)/4;

   //Let threads wait till they all finish their work.
   //We would probably use the barrier here

  /*Start idea is that we have each thread take a quarter of the image
   *and use that as its size parameter. The formula is (w * h)/# of threads 
   */

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
    PPMPixel* img = readImage(argv[1], &w, &h);
    PPMPixel* finalImg = apply_filters(img, w, h, &elapsedTime);

    //test image
    char* name = "laplacian.ppm";
    writeImage(img, name, w, h);

	return 0;
}
