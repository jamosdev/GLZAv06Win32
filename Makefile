CFLAGS=/I. /DBUILD_COMBINED_EXE /experimental:c11atomics /std:c11
CC=cl
SRC=GLZAcompress.c\
GLZAdecode.c\
GLZAencode.c\
GLZAformat.c\
glzamain.c
HEADERS=GLZAmodel.h\
threading_and_main.h
OBJS=glzacomp.obj\
glzadeco.obj\
glzaenco.obj\
glzafrmt.obj\
glzamain.obj
RM=del
all: glza
clean:
	-$(RM) glzacomp.obj
	-$(RM) glzadeco.obj
	-$(RM) glzaenco.obj
	-$(RM) glzafrmt.obj
	-$(RM) glzamain.obj
glzacomp.obj:GLZAcompress.c $(HEADERS)
	cl /I. /DBUILD_COMBINED_EXE /experimental:c11atomics /std:c11 /c GLZAcompress.c /Fo$@
glzadeco.obj:GLZAdecode.c $(HEADERS)
	cl /I. /DBUILD_COMBINED_EXE /experimental:c11atomics /std:c11 /c GLZAdecode.c /Fo$@
glzaenco.obj:GLZAencode.c $(HEADERS)
	cl /I. /DBUILD_COMBINED_EXE /experimental:c11atomics /std:c11 /c GLZAencode.c /Fo$@
glzafrmt.obj:GLZAformat.c $(HEADERS)
	cl /I. /DBUILD_COMBINED_EXE /experimental:c11atomics /std:c11 /c GLZAformat.c /Fo$@
glzamain.obj:glzamain.c
	cl /I. /experimental:c11atomics /std:c11 /c glzamain.c /Fo$@
glza: glza.exe
glza.exe: $(OBJS)
	cl $(CFLAGS) $(OBJS) /Fe$@
