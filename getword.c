#include <string.h>
#include <ctype.h>

#define LETTER 'a'
#define EOW ','
#define MAXLEN 50

getword(w, lim, fp)
char *w;
FILE *fp;
{
    extern ch(), unch();
    int c;

    if (!isalpha(c = *w++ = ch(fp))) {
        *w = '\0';
        return c;
    }
    while (--lim > 0) {
        c = *w++ = ch(fp);
        if (!isalpha(c) && !isdigit(c) && c != ' ') {
            unch(c, fp);
            break;
        }
    }
    *(w-1) = '\0';
    return LETTER;
}

char *strsave(s)
char *s;
{
    char *p;
    
    if ((p = (char *) malloc(strlen(s)+1)) != NULL)
        strcpy(p, s);
    return p;
}
