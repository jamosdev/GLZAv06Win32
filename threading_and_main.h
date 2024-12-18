
#ifdef BUILD_COMBINED_EXE
#ifndef SOURCECODE_FILENAME
#error define SOURCECODE_FILENAME before including threading_and_main.h (if using glzamain.c all-in-one)
#endif
#define main(a,b) z_main(a,b,SOURCECODE_FILENAME)
#define z_main(a,b,c) zz_main(a,b,c)
#define zz_main(a,b,c) c##_main(a,b)
#endif

#ifdef _MSC_VER
#include <jthread.h>
#else
#include <pthread.h>
#endif
