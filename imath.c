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

//pthread_barrier_t our_barrier;

/*This is the thread function. It will compute the new values for the region of image specified in params (start to start+size) using convolution.
    (1) For each pixel in the input image, the filter is conceptually placed on top ofthe image with its origin lying on that pixel.
    (2) The  values  of  each  input  image  pixel  under  the  mask  are  multiplied  by the corresponding filter values.
    (3) The results are summed together to yield a single output value that is placed in the output image at the location of the pixel being processed on the input.
 
 */
void *threadfn(void *params)
{
	struct parameter* p = (struct parameter*) params;
	int laplacian[FILTER_WIDTH][FILTER_HEIGHT] =
	{
	  -1, -1, -1,
	  -1,  8, -1,
	  -1, -1, -1,
	};
  int red, green, blue;
  int x_coordinate = 0;
  int y_coordinate = 0;
  int iteratorImageWidth = 0;
  int iteratorImageHeight = 0;
  int iteratorFilterWidth = 0;
  int iteratorFilterHeight = 0;
  
  for(iteratorImageHeight = p->start; iteratorImageHeight < p->size; iteratorImageHeight++)
  {
    for(iteratorImageWidth = 0; iteratorImageWidth < p->w; iteratorImageWidth++)
    {
      red = 0;
      green = 0;
      blue = 0;
      for(iteratorFilterHeight = 0; iteratorFilterHeight < FILTER_HEIGHT; iteratorFilterHeight++)
      {
        for(iteratorFilterWidth = 0; iteratorFilterWidth < FILTER_WIDTH; iteratorFilterWidth++)
        {
          //depending on which filter height and width, get that corresponding pixel
          x_coordinate = (iteratorImageWidth - (FILTER_WIDTH / 2) + iteratorFilterWidth + p->w) % p->w;
          y_coordinate = (iteratorImageHeight - (FILTER_HEIGHT / 2) + iteratorFilterHeight + p->h) % p->h;
          red += (int)p->image[y_coordinate * p->w + x_coordinate].r * laplacian[iteratorFilterHeight][iteratorFilterWidth];
          green += (int)p->image[y_coordinate * p->w + x_coordinate].g * laplacian[iteratorFilterHeight][iteratorFilterWidth];
          blue += (int)p->image[y_coordinate * p->w + x_coordinate].b * laplacian[iteratorFilterHeight][iteratorFilterWidth];
        }
      }        
      if(red > 255)
        p->result[iteratorImageHeight * p->w + iteratorImageWidth].r = 255;
      else if(red < 0)
        p->result[iteratorImageHeight * p->w + iteratorImageWidth].r = 0;
      else
        p->result[iteratorImageHeight * p->w + iteratorImageWidth].r = red;

      if(green > 255)
        p->result[iteratorImageHeight * p->w + iteratorImageWidth].g = 255;
      else if(green < 0)
        p->result[iteratorImageHeight * p->w + iteratorImageWidth].g = 0;
      else
        p->result[iteratorImageHeight * p->w + iteratorImageWidth].g = green;

      if(blue > 255)
        p->result[iteratorImageHeight * p->w + iteratorImageWidth].b = 255;
      else if(blue < 0)
        p->result[iteratorImageHeight * p->w + iteratorImageWidth].b = 0;
      else
        p->result[iteratorImageHeight * p->w + iteratorImageWidth].b = blue;
    }
    //printf("%d\n", iteratorImageHeight);
  }

  /*For all pixels in the work region of image (from start to start+size)
    Multiply every value of the filter with corresponding image pixel. Note: this is NOT matrix multiplication.
   
   //truncate values smaller than zero and larger than 255
    Store the new values of r,g,b in p->result.
   */
	//pthread_barrier_wait(&our_barrier);	
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
  //printf("%ld %ld \n", width, height);

  //Writing the header block

  fprintf(fp, "P6\n %ld %ld\n 255\n", width, height); 
  //add the values by row
  
  size_t check;
  int counter = 0;

  for(int i = 0; i < (height * width); i++)
  {
    counter++;
    //printf("%d %d %d %d\n", image[i].r, image[i].g, image[i].b, counter);
    
    check = fputc(image[i].r, fp);
    if(check == EOF){
      //error
      printf("Error in writeImage1\n");
      fclose(fp);
      exit(1);
    }
    check = fputc(image[i].g, fp);
    if(check == EOF){
      //error
      printf("Error in writeImage2\n");
      fclose(fp);
      exit(1);
    }
    check = fputc(image[i].b, fp);
    if(check == EOF){
      //error
      printf("Error in writeImage3\n");
      fclose(fp);
      exit(1);
    }
  }
  fclose(fp);
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

  //PPMPixel *img;
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

  //check for max byte
  //Read rgb component.  Check if it is equal toman pthread RGB_MAX. If not, display error message.
  fscanf(fp, "%d", &fileMaxRGB);
  check = fread(&input, sizeof(char), 1, fp);
  if(check == 0)
    printf("error first\n");

  //read pixel data from filename into img. The pixel data is stored in scanline order from left to right (up to bottom) in 3-byte chunks (r g b values for each pixel) encoded as binary numbers.
  PPMPixel* pix;
  //allocate memory for img. NOTE: A ppm image of w=200 and h=300 will contain 60000 triplets (i.e. for r,g,b), ---> 18000 bytes.
  pix = (PPMPixel*)malloc(sizeof(PPMPixel) * (*height) * (*width));

  //Takes the rgb values of each pixel and stores them into the array pix
  for(int i = 0; i < ((*width) * (*height)); i++)
  {
    pix[i].r = fgetc(fp);
    pix[i].g = fgetc(fp);
    pix[i].b = fgetc(fp);
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
  struct timeval t1, t2;
  gettimeofday(&t1, NULL);
  result = (PPMPixel*)malloc(sizeof(PPMPixel) * h * w);
  struct parameter params[THREADS];
  pthread_t threadArray[THREADS];
  for(int i = 0; i < THREADS; i++)
  {
    //initialize individual values for the threads
    params[i].image = image;
    params[i].result = result;
    params[i].w = w;
    params[i].h = h;
    params[i].start = (h/THREADS) * i;
    if(i == THREADS - 1)
    {
      params[i].size = h;
    }
    else
    {
      params[i].size = h/THREADS * (i + 1);
    }
    if(pthread_create(&threadArray[i], NULL, threadfn, (void*)&params[i]) != 0)
    {
      printf("ERROR CREATING THREAD\n");
      exit(1);
    }
  }

  for(int i = 0; i < THREADS; i++)
  {
    //join the threads
    pthread_join(threadArray[i], NULL);
  }
  gettimeofday(&t2, NULL);
  *elapsedTime = (t2.tv_sec - t1.tv_sec) + ((t2.tv_usec - t1.tv_usec)/1000000.0);
	return result;
}


/*The driver of the program. Check for the correct number of arguments. If wrong print the message: "Usage ./a.out filename"
    Read the image that is passed as an argument at runtime. Apply the filter. Print elapsed time in .3 precision (e.g. 0.006 s). Save the result image in a file called laplacian.ppm. Free allocated memory.
 */
int main(int argc, char *argv[])
{
  //load the image into the buffer
    unsigned long int w, h;
    double elapsedTime = 0.0;
    
    if(argc < 2)
    {
      printf("Usage ./imath filename\n");
    }

    int i = 0;
    double totalTime;
    double averageTime;

    while(i < 50){
      PPMPixel* img = readImage(argv[1], &w, &h);
      PPMPixel* finalImg = apply_filters(img, w, h, &elapsedTime);
      if(elapsedTime > 0){
        totalTime += elapsedTime;
        printf("elapsedTime: %.3f\n", elapsedTime);
        i++;
      }

      elapsedTime = 0;

      char* name = "laplacian.ppm";
      writeImage(finalImg, name, w, h);

      free(img);
      free(finalImg);
    }
    averageTime = totalTime/50;
    printf("Average time over 50 runs: %.3f\n", averageTime);

	return 0;
}
