#pragma once

#ifdef LIBOS_EXPORTS
#   define LIBOS_API __declspec(dllexport)
#else
#   define LIBOS_API __declspec(dllimport)
#endif
