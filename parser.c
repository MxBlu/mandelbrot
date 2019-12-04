/*
   HTTP Request Parser

   Parses HTTP requests into appropriate data structs

   Synax to be parsed is defines as EXPECTED_FORMAT where [x] and [y] are
   double and [z] is an integer

   trueFalse.h contains defines to emulate boolean variables, and must be
   included with this file

   By: Jumail Mundekkat
*/

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "trueFalse.h"
#include "parser.h"

#define DOUBLE_BUFFER_SIZE 32
#define INT_BUFFER_SIZE 8

#define EXPECTED_FORMAT "/tile_x[x]_y[y]_z[z].bmp"

int decodeRequest (char * request, int requestSize, char * decodeArray, int decodeSize) {

   int decodeResult = 0;
   int requestEndFlag = FALSE;

   int requestArrayCounter = 0;
   int decodeArrayCounter = 0;
   int decodeArrayEnd = 0;

   assert ( requestSize > 4 );

   if ( request[0] == 'G' &&
      request[1] ==   'E' &&
      request[2] ==   'T' &&
      request[3] ==   ' ' &&
      request[4] ==   '/') {

      requestArrayCounter = 4;
      while (requestArrayCounter < requestSize && requestEndFlag == FALSE) {
         if (request[requestArrayCounter] != ' ') {

            assert (decodeArrayCounter < decodeSize);
            decodeArray[decodeArrayCounter] = request[requestArrayCounter];

            requestArrayCounter++;
            decodeArrayCounter++;
         } else {
            requestEndFlag = TRUE;
         }
      }

      decodeArrayEnd = decodeArrayCounter - 1;

      if (requestEndFlag == FALSE) {
         decodeResult = BAD_REQUEST;

      } else if (decodeArrayCounter > 4) {

         if ( decodeArray[decodeArrayEnd - 3] == '.' &&
            decodeArray[decodeArrayEnd - 2] ==   'b' &&
            decodeArray[decodeArrayEnd - 1] ==   'm' &&
            decodeArray[decodeArrayEnd] ==       'p' ) {

            decodeResult = BMP_REQUEST;
         } else {
            decodeResult = HTML_REQUEST;
         }

      } else {
         decodeResult = HTML_REQUEST;
      }

   } else {
      decodeResult = BAD_REQUEST;
   }

   return decodeResult;
}

int parseMandelbrot (char * decodedRequest, int decodedSize, MandelbrotRequest * outRequest) {
   int parseResult = TRUE;

   char * expectedString = EXPECTED_FORMAT;
   int decodedCounter = 0;
   int expectedCounter = 0;

   int expectedSize = strlen(expectedString);

   assert ( expectedSize > 0 );
   assert ( decodedSize > 0 );

   while ( decodedCounter < decodedSize && expectedCounter < expectedSize  && parseResult != FALSE) {


      if ( expectedString[expectedCounter] == '[') {
         expectedCounter++;

         assert ( expectedCounter + 1 < expectedSize );
         assert ( expectedString[expectedCounter] == 'x' ||
                  expectedString[expectedCounter] == 'y' ||
                  expectedString[expectedCounter] == 'z');

         char * returnAddr = NULL;

         if ( expectedString[expectedCounter] == 'x' ) {
            outRequest -> x = strtod(&decodedRequest[decodedCounter], &returnAddr);

         } else if ( expectedString[expectedCounter] == 'y' ) {
            outRequest -> y = strtod(&decodedRequest[decodedCounter], &returnAddr);

         } else if ( expectedString[expectedCounter] == 'z' ) {
            outRequest -> z = strtol(&decodedRequest[decodedCounter], &returnAddr, 10);

         }

         decodedCounter += returnAddr - &decodedRequest[decodedCounter];
         expectedCounter += 2;

      } else if (decodedRequest[decodedCounter] == expectedString[expectedCounter] || expectedString[expectedCounter] == '\0') {
         expectedCounter++;
         decodedCounter++;

      } else {
         parseResult  = FALSE;

      }
   }

   return parseResult;
}
