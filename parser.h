#ifndef PARSER_H_INCLUDED
   #define PARSER_H_INCLUDED

	#define BAD_REQUEST -1
	#define HTML_REQUEST 1
	#define BMP_REQUEST 2

	typedef struct _mandelbrotRequest {

		double x;
		double y;
		int z;

	} MandelbrotRequest;

	int decodeRequest (char * request, int requestSize, char * decodeArray, int decodeSize);
	// Parses a HTTP GET request and stores the requested file to a
	// char array

	// Input is a pointer to the HTTP request, its respective size,
	// a pointer to a char array to store the requested file and
	// its respective size

	// Returns TRUE or FALSE based on if it matched the syntax

	// decodeArray is a terminated char array after being passed
	// through this function.

	int parseMandelbrot (char * decodedRequest, int decodedSize, MandelbrotRequest * outRequest);
	// Parses a requested filename by comparing it against EXPECTED_FORMAT

	// Input is a pointer to the requested filename, its respective size,
	// a pointer to a MandelbrotRequest, holding a triordinate

	// Returns TRUE or FALSE based on if it matched the syntax

#endif //PARSER_H_INCLUDED
