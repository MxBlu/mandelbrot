#ifndef MULTIPLATFORM_H_INLCUDED
   #define MULTIPLATFORM_H_INCLUDED

   // Allows for use of the server code on Windows as well as Unix

   #define PLATFORM_WINDOWS  1
   #define PLATFORM_MAC      2
   #define PLATFORM_UNIX     3

   #if defined(_WIN32)
      #define PLATFORM PLATFORM_WINDOWS
   #elif defined(__APPLE__)
      #define PLATFORM PLATFORM_MAC
   #else
      #define PLATFORM PLATFORM_UNIX
   #endif

   #if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
      #include <netinet/in.h>
      typedef unsigned char* packet;
   #elif PLATFORM == PLATFORM_WINDOWS
      #include <winsock2.h>
      typedef int socklen_t;
      typedef const char* packet;
   #endif

#endif // MULTIPLATFORM_H_INLCUDED
