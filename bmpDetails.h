#ifndef BMPDETAILS_H_INCLUDED
   #define BMPDETAILS_H_INCLUDED

   /*
      BMP Header Details

      Contains defines for values in the header of a bitmap file.
   */

   #define BYTES_PER_PIXEL 3
   #define BITS_PER_PIXEL (BYTES_PER_PIXEL*8)
   #define NUMBER_PLANES 1
   #define PIX_PER_METRE 2835
   #define MAGIC_NUMBER 0x4d42
   #define NO_COMPRESSION 0
   #define OFFSET 54
   #define DIB_HEADER_SIZE 40
   #define NUM_COLORS 0

   #define HEADER_SIZE 54
   #define SIZE 512

   typedef unsigned char  bits8;
   typedef unsigned short bits16;
   typedef unsigned int   bits32;

#endif //BMPDETAILS_H_INCLUDED
