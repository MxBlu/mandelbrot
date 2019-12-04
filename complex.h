#ifndef COMPLEX_H_INCLUDED
   #define COMPLEX_H_INCLUDED

	typedef struct _myComplex {

	   double real;
	   double img;

	} Complex;

	Complex addC(Complex a, Complex b);
	// Adds two complex numbers in the form of Complex structs
	//	 (a + bi) + (c + di) = (a + c) + (b + d)i
	// Returns a Complex struct

	Complex multiC(Complex a, Complex b);
	// Multiplies two complex numbers in the form of Complex structs
	//	 (a + bi) * (c + di) = (ac + bd) + (ad + bc)i
	// Returns a Complex struct

	double distC (Complex a);
	// Calculates the distance of a complex number from the origin
	// on a plane wtih a real and imaginary axis
	// 	 dist(a + bi) = sqrt( a^2 + b^2 )
	// Returns a double

	void printC (Complex a);
	// Prints out a Complex struct in the form of (a, b) where
	// complex number is in the form a + bi

#endif //COMPLEX_H_INCLUDED
