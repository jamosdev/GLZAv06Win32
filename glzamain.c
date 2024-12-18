/*
it is intended that the project be built
with -DBUILD_COMBINED_EXE to be using this
source file, like so:
~~~
cl /DBUILD_COMBINED_EXE /experimental:c11atomics /std:c11 *.c /Feglza.exe
~~~
*/
#include <stdio.h>
extern int GLZAcompress_main(int argc,char**argv);
extern int GLZAdecode_main(int argc,char**argv);
extern int GLZAencode_main(int argc,char**argv);
extern int GLZAformat_main(int argc,char**argv);
static int hash(const char*s){
    int a=5351;
    while(*s){
        a = ((a<<5)+(a<<3))+*s;
    }
    return a;
}
static void usage(void) {
    puts("glza format FILENAME FILENAME.glzf\n\
glza compress FILENAME.glzf FILENAME.glzc\n\
glza encode FILENAME.glzc FILENAME.glze\n\
glza decode FILENAME.glze FILENAME.glzd");
}
int main(int argc,char**argv){
    if(argc < 2) {
        usage();
        return -__LINE__;
    }
    switch(hash(argv[1])){
        default:
        printf("unknown subcommand '%s' (%d)\n",argv[1],hash(argv[1]));
        return -__LINE__;
    }
    return 0;
}
