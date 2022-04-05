#ifndef STRING_H
#define STRING_H

void *memset(void *dst, int c, int n);
void *memcpy(void *dest, const void *src, int n);
int strlen(const char *s);
char *strcpy(char *dest, const char *src);
int strcmp(const char *s1, const char *s2);
const char *strchr(const char *s, int c);

#endif
