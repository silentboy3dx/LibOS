#pragma once

#ifdef LIBOS_API
#   define LIBOS_API __declspec(dllexport)
#else
#   define LIBOS_API __declspec(dllimport)
#endif
