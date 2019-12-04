/*
   Complex Number definition and function

   Add, multiply and modulo function (named distC) for complex numbers

   Also contains a printC function to print complex numbers

   By: Jumail & Zihan
   11/04/16 6:21PM
*/

#include <stdio.h>
#include <math.h>

#include "complex.h"

Complex addC (Complex a, Complex b) {
   Complex result = { a.real + b.real,
                      a.img + b.img };

   return ( result );
}

Complex multiC (Complex a, Complex b) {
   Complex result = { (a.real*b.real - a.img*b.img),
                      (a.real*b.img + b.real*a.img) };

   return ( result );
}

void printC (Complex a) {
   printf( "(%.4lf, %.4lf)", a.real, a.img );
   return;
}

double distC (Complex a) {
   double result = sqrt( pow(a.real, 2) + pow(a.img, 2) );
   return ( result );
}
