/*
it is intended that the project be built
with -DBUILD_COMBINED_EXE to be using this
source file, like so:
~~~
cl /DBUILD_COMBINED_EXE /experimental:c11atomics /std:c11 *.c /Feglza.exe
~~~
*/
#include <stdio.h>
#include <ctype.h>
extern int GLZAcompress_main(int argc,char**argv);
extern int GLZAdecode_main(int argc,char**argv);
extern int GLZAencode_main(int argc,char**argv);
extern int GLZAformat_main(int argc,char**argv);
static int hash(const char*s){
    int a=5351;
    while(*s){
        a = ((a<<5)+(a<<3))+tolower(*s);
        ++s;
    }
    return a;
}
static void usage(void) {
    puts("glza format FILENAME FILENAME.glzf\n\
glza compress FILENAME.glzf FILENAME.glzc\n\
glza encode FILENAME.glzc FILENAME.glze\n\
glza decode FILENAME.glze FILENAME.glzd");
}
#define HASH_OF_format  -1670578980
#define HASH_OF_compress 441277867
#define HASH_OF_encode  -1776495675
#define HASH_OF_decode  -1901935675
//#include <stdlib.h>
int main(int argc,char**argv){
   // system("cd");
    if(argc < 2) {
        usage();
        return -__LINE__;
    }
    switch(hash(argv[1])){
    case HASH_OF_format:
        return GLZAformat_main(argc-1,argv+1);
        break;
    case HASH_OF_compress:
        return GLZAcompress_main(argc-1,argv+1);
        break;
    case HASH_OF_encode:
        return GLZAencode_main(argc-1,argv+1);
        break;
    case HASH_OF_decode:
        return GLZAdecode_main(argc-1,argv+1);
        break;
    default:
        printf("unknown subcommand '%s' (%d)\n",argv[1],hash(argv[1]));
        return -__LINE__;
    }
    return 0;
}
