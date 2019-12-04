/*
   Bitmap Webserver

   Contains various functions to create a socket, wait for a TCP
   connection, and serve a HTML or BMP to said connection

   Also contains a function to create a bitmap header file from
   the defines in bmpDetails.h

   NOTE:    The multi-platform code will only work as intended under MinGW
            and the Unix native GCC compiler. It may potentially work under
            MSVC. No other compiler is guaranteed to work.

   WARNING: serveHTML currently does not work the same under MinGW as it
            does under GCC, due to an unknown reason. Use with caution.

   By: Jumail Mundekkat

   Base code by Tim Lambert 02/04/12
   from his bmpServer.c
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

#include "multiplatform.h"
#include "bmpDetails.h"
#include "bmpServer.h"

void serveBMP (int socket, unsigned char * bmp, int bmpSize) {

   char* header = "HTTP/1.0 200 OK\r\n"
                "Content-Type: image/bmp\r\n"
                "\r\n";

   // first send the http response header

   printf ("about to send=> \n%s\n", header);

   int bytesWritten;
   bytesWritten = send (socket, (packet) header, strlen (header), 0);

   // now send the BMP
   bytesWritten = send (socket, (packet) bmp, bmpSize, 0);

   printf("%d bytes written to socket.\n", bytesWritten);

   return;
}

void serveHTML(int socket, char * message, int messageSize) {
   int bytesWritten;

   // echo the http response to the console for debugging purposes
   char* header = "HTTP/1.0 200 OK\r\n"
                "Content-Type: text/html\r\n"
                "\r\n";

   bytesWritten = send (socket, (packet) header, strlen(header), 0);

   printf ("about to send=> \n%s\n", message);


   // send the http response to the web browser which requested it
   bytesWritten = send (socket, (packet) message, messageSize, 0);

   printf("%d bytes written to socket.\n", bytesWritten);

   return;
}

int makeServerSocket (int portNumber) {

   // create socket

   #if PLATFORM == PLATFORM_WINDOWS
      WSADATA WsaData;
      assert(!(WSAStartup( MAKEWORD(2,2), &WsaData )));
   #endif

   int serverSocket = socket (AF_INET, SOCK_STREAM, 0);
   assert (serverSocket >= 0);
   // error opening socket

   // bind socket to listening port
   struct sockaddr_in serverAddress;
   memset ((char *) &serverAddress, 0,sizeof (serverAddress));

   serverAddress.sin_family      = AF_INET;
   serverAddress.sin_addr.s_addr = INADDR_ANY;
   serverAddress.sin_port        = htons (portNumber);

   // let the server start immediately after a previous shutdown
   #if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
      const int optionValue = 1;
   #elif PLATFORM == PLATFORM_WINDOWS
      const char optionValue = 1;
   #endif

   setsockopt (
      serverSocket,
      SOL_SOCKET,
      SO_REUSEADDR,
      &optionValue,
      sizeof(int)
   );

   int bindSuccess =
      bind (
         serverSocket,
         (struct sockaddr *) &serverAddress,
         sizeof (serverAddress)
      );

   assert (bindSuccess >= 0);
   // if this assert fails wait a short while to let the operating
   // system clear the port before trying again

   return serverSocket;
}

int waitForConnection (int serverSocket) {
   // listen for a connection
   const int serverMaxBacklog = 10;
   listen (serverSocket, serverMaxBacklog);

   // accept the connection
   struct sockaddr_in clientAddress;
   socklen_t clientLen = sizeof (clientAddress);
   int connectionSocket =
      accept (
         serverSocket,
         (struct sockaddr *) &clientAddress,
         &clientLen
      );

   assert (connectionSocket >= 0);
   // error on accept

   return (connectionSocket);
}

void makeBmpHeader (unsigned char * headerArray) {
   assert(sizeof (bits8) == 1);
   assert(sizeof (bits16) == 2);
   assert(sizeof (bits32) == 4);

   int index = 0;

   bits16 magicNumber = MAGIC_NUMBER;
   memcpy(&headerArray[index], &magicNumber, sizeof(magicNumber));
   index += sizeof(magicNumber);

   bits32 fileSize = OFFSET + (SIZE * SIZE * BYTES_PER_PIXEL);
   memcpy(&headerArray[index], &fileSize, sizeof(fileSize));
   index += sizeof(fileSize);

   bits32 reserved = 0;
   memcpy(&headerArray[index], &reserved, sizeof(reserved));
   index += sizeof(reserved);

   bits32 offset = OFFSET;
   memcpy(&headerArray[index], &offset, sizeof(offset));
   index += sizeof(offset);

   bits32 dibHeaderSize = DIB_HEADER_SIZE;
   memcpy(&headerArray[index], &dibHeaderSize, sizeof(dibHeaderSize));
   index += sizeof(dibHeaderSize);

   bits32 width = SIZE;
   memcpy(&headerArray[index], &width, sizeof(width));
   index += sizeof(width);

   bits32 height = SIZE;
   memcpy(&headerArray[index], &height, sizeof(height));
   index += sizeof(height);

   bits16 planes = NUMBER_PLANES;
   memcpy(&headerArray[index], &planes, sizeof(planes));
   index += sizeof(planes);

   bits16 bitsPerPixel = BITS_PER_PIXEL;
   memcpy(&headerArray[index], &bitsPerPixel, sizeof(bitsPerPixel));
   index += sizeof(bitsPerPixel);

   bits32 compression = NO_COMPRESSION;
   memcpy(&headerArray[index], &compression, sizeof(compression));
   index += sizeof(compression);

   bits32 imageSize = (SIZE * SIZE * BYTES_PER_PIXEL);
   memcpy(&headerArray[index], &imageSize, sizeof(imageSize));
   index += sizeof(imageSize);

   bits32 hResolution = PIX_PER_METRE;
   memcpy(&headerArray[index], &width, sizeof(hResolution));
   index += sizeof(hResolution);

   bits32 vResolution = PIX_PER_METRE;
   memcpy(&headerArray[index], &vResolution, sizeof(vResolution));
   index += sizeof(vResolution);

   bits32 numColors = NUM_COLORS;
   memcpy(&headerArray[index], &numColors, sizeof(numColors));
   index += sizeof(numColors);

   bits32 importantColors = NUM_COLORS;
   memcpy(&headerArray[index], &importantColors, sizeof(importantColors));
   index += sizeof(importantColors);

   return;
}
