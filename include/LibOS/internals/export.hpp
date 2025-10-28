#pragma once

#ifdef _WIN32
  #ifdef LIBOS_API
    #define LIBOS_API __declspec(dllexport)
  #else
    #define LIBOS_API __declspec(dllimport)
  #endif
#else
  #define LIBOS_API
#endif
