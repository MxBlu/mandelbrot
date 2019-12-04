#ifndef MANDELBROT_H_INCLUDED
   #define MANDELBROT_H_INCLUDED

   /*
    *  mandelbrot.h
    *  mandelbrot server
    *
    *  Created by Richard Buckland on 13/04/13.
    *  Licensed under Creative Commons BY 3.0.
    *
    */

   void testEscapeSteps(void);
   int escapeSteps (double x, double y);
   void getMandelbrot (MandelbrotRequest inRequest, unsigned char * byteArray, int arraySize);

#endif // MANDELBROT_H_INCLUDED
