#include <stdint.h>

void * memset (void *dst, int c, int len)
{
  uint8_t *ptr = dst;
  while (len-- > 0)
    *ptr++ = c;
  return dst;
}

void * memcpy (void *dst, const void *src, int len)
{
  char *d = dst;
  const char *s = src;
  while (len--)
    *d++ = *s++;
  return dst;
}

int strlen(const char *s)
{
    const char *str;
    
    for(str = s; *str; str++);
    return(str - s);
}

char *strcpy(char *dst, const char *src)
{
    char *res = dst;
    do
        *res++ = *src;
    while(*src++);
    return dst;
}

int strcmp(const char *s1, const char *s2)
{
	while (*s1 == *s2++)
		if (*s1++ == 0)
			return (0);
	return (*(unsigned char *)s1 - *(unsigned char *)--s2);
}

const char *strchr(const char *s, int c)
{
    char res;

	res = c;
	for (;; ++s) 
    {
		if (*s == res)
			return ((char *)s);
		if (*s == '\0')
			return (0);
	}
}



