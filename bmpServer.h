#ifndef BMPSERVER_H_INCLUDED
   #define BMPSERVER_H_INCLUDED

   int waitForConnection (int serverSocket);
   int makeServerSocket (int portno);

   void serveBMP (int socket, unsigned char * bmp, int bmpSize);
   // Sends a reply to the socket stating an bitmap is to be sent
   // and sends a bitmap

   // Bitmap must be complete, containing the header.

   void serveHTML(int socket, char * message, int messageSize);
   // Sends a reply to the socket stating a HTML page is to be sebt
   // and sends a HTML page

   void makeBmpHeader (unsigned char * headerArray);
   // Generates a bitmap header based on bitmapDetails.h and stores it
   // in the passed-in array

#endif // BMPSERVER_H_INCLUDED
