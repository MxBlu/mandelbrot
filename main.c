/*
   Mandelbrot Webserver

   Serves images of Mandelbrot set to a browser given the x and y
   coordinates as well as the zoom

   WARNING: Caching will occupy a large amount of space due to the
            large amounts of ~1MB bitmap files generated by the server.
            Use with caution as there is no protection from it using up
            all of your disk space.

   WARNING: serveHTML currently does not work the same under MinGW as it
            does under GCC, due to an unknown reason. Use with caution.

   NOTE:    The multi-platform code will only work as intended under MinGW
            and the Unix native GCC compiler. It may potentially work under
            MSVC. No other compiler is guaranteed to work.

   By: Jumail Mundekkat

   Base code by Tim Lambert 02/04/12
   from his bmpServer.c
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

#include "multiplatform.h"
#include "trueFalse.h"
#include "bmpDetails.h"
#include "complex.h"
#include "parser.h"
#include "mandelbrot.h"
#include "bmpServer.h"

#define MANDELBROT_SERVER_VERSION 2.42 //increment on update ;)
#define REQUEST_BUFFER_SIZE 1000
#define DEFAULT_PORT 18002
#define NUMBER_OF_PAGES_TO_SERVE 1000
// after serving this many pages the server will halt

#define CACHING_ENABLED 0

int main (int argc, char *argv[]) {
   //testEscapeSteps();

   printf ("************************************\n");
   printf ("Starting Mandelbrot %f\n", MANDELBROT_SERVER_VERSION);
   printf ("Serving bmps since 2016\n");

   int serverSocket = makeServerSocket (DEFAULT_PORT);
   printf ("Access this server at http://localhost:%d/\n", DEFAULT_PORT);
   printf ("************************************\n");

   char request[REQUEST_BUFFER_SIZE];
   unsigned char bmpHeader[HEADER_SIZE];

   makeBmpHeader(&bmpHeader[0]);

   int numberServed = 0;
   while (numberServed < NUMBER_OF_PAGES_TO_SERVE) {

      printf ("*** So far served %d pages ***\n", numberServed);

      int connectionSocket = waitForConnection (serverSocket);
      // wait for a request to be sent from a web browser, open a new
      // connection for this conversation

      // read the first line of the request sent by the browser
      int bytesRead;
      bytesRead = recv (connectionSocket, request, sizeof(request) - 1, 0);
      assert (bytesRead >= 0);
      // were we able to read any data from the connection?

      // print entire request to the console
      //printf (" *** Received http request ***\n %s\n", request);

      int decodeResult;
      char decodedArray[REQUEST_BUFFER_SIZE] = {0};
      decodeResult = decodeRequest(&request[0], REQUEST_BUFFER_SIZE, &decodedArray[0], REQUEST_BUFFER_SIZE);

      if (decodeResult == HTML_REQUEST) {
         //send the browser the a page calling the tiles.js script
         printf (" *** Sending http response ***\n");

         char * message =
            "<!DOCTYPE html>\n"
            "<html>\n"
            "   <script src=\"http://almondbread.cse.unsw.edu.au/tiles.js\"></script>"
            "</html>\n";

         serveHTML(connectionSocket, message, strlen(message));

      } else if (decodeResult == BMP_REQUEST) {

         //test if the request follows the syntax
         MandelbrotRequest mandelbrotIn = {0.00f, 0.00f, 0};
         int parseSuccess = parseMandelbrot (&decodedArray[0], REQUEST_BUFFER_SIZE, &mandelbrotIn);

         //if it is a valid request, serve the tile given the triorinates
         if (parseSuccess == TRUE) {
            unsigned char byteArray [SIZE * SIZE * BYTES_PER_PIXEL + HEADER_SIZE] = {0};

            //check if caching is enabled
            if (CACHING_ENABLED) {
               if( access( &decodedArray[1], F_OK ) != -1 ) {
                  int fileBytesRead = 0;
                  //a cached copy exists, read it to the array
                  printf("reading from cache\n");

                  FILE * cache = fopen(&decodedArray[1], "rb");
                  fileBytesRead = fread (&byteArray[0], SIZE * SIZE * BYTES_PER_PIXEL + HEADER_SIZE, 1, cache);
                  printf("%d bytes read from cache\n", fileBytesRead);
                  fclose(cache);

               } else {
                  //no cached copy exists, generate the image and cache it
                  //before sending it
                  memcpy(&byteArray[0], &bmpHeader[0], HEADER_SIZE);
                  getMandelbrot(mandelbrotIn, &byteArray[HEADER_SIZE], (SIZE * SIZE * BYTES_PER_PIXEL));

                  printf("no cache available\n");

                  FILE * cache = fopen(&decodedArray[1], "wb");
                  fwrite (&byteArray[0], SIZE * SIZE * BYTES_PER_PIXEL + HEADER_SIZE, 1, cache);
                  fclose(cache);
               }
            } else {
               memcpy(&byteArray[0], &bmpHeader[0], HEADER_SIZE);
               getMandelbrot(mandelbrotIn, &byteArray[HEADER_SIZE], (SIZE * SIZE * BYTES_PER_PIXEL));
            }

            //send over the loaded image
            serveBMP(connectionSocket, &byteArray[0], (SIZE * SIZE * BYTES_PER_PIXEL) + HEADER_SIZE);

         //otherwise, complain nicely
         } else {
            char * message =
              "<!DOCTYPE html>\n"
              "<html>\n"
              "   <head><title>Error</title></head>\n"
              "   <body>Unknown format, please use /tile_x(double)_y(double)_z(integer).bmp</body>\n"
              "</html>\n";

           serveHTML(connectionSocket, message, strlen(message));
         }
      }

      // close the connection after sending the page- keep aust beautiful
      close(connectionSocket);

      numberServed++;
   }

   // close the server connection after we are done- keep aust beautiful
   printf ("** shutting down the server **\n");

   #if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
      close( serverSocket );
   #elif PLATFORM == PLATFORM_WINDOWS
      closesocket( serverSocket );
      WSACleanup();
   #endif

   return EXIT_SUCCESS;
}