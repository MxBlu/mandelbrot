/*
   Pixel Color Functions

   Given the iterations of the Mandelbrot set to escape, returns
   int intensity of its respective color

   By: Jumail Mundekkat
*/

#include <math.h>

#include "pixelColor.h"

#ifndef M_PI
   #define M_PI 3.14159265358979323846
#endif
//Stops MinGW from complaining

unsigned char stepsToRed (int steps) {
   unsigned char outColor = 0;
   outColor = 253*sin(256*steps/M_PI) + 2*sin(128*steps/M_PI) + 257/2;

   return outColor;
}
unsigned char stepsToBlue (int steps) {
   unsigned char outColor = 0;
   outColor = 253*sin(256*steps/M_PI + 2*M_PI/3) + 2*sin(128*steps/M_PI) + 257/2;

   return outColor;
}
unsigned char stepsToGreen (int steps) {
   unsigned char outColor = 0;
   outColor = 253*sin(256*steps/M_PI + 4*M_PI/3) + 2*sin(128*steps/M_PI) + 257/2;

   return outColor;
}
