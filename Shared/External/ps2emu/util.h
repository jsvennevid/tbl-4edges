#ifndef ps2emu_util_h
#define ps2eum_util_h

namespace ps2emu
{

void        setdata(void* a, int b, int c, int bits);
long        iToBin(char value);
int         round(long double a);
char*       strtrim(char* a);
long        htol(char* a);

#ifndef WIN32
char * strupr(char *);
char * strrev(char *);
char * strlwr(char *);
#endif

}

#endif
