#include <pebble.h>

char* int_to_string(char str[], int num)
{
    int i, rem, len = 0, n;
 
    n = num;
    while (n != 0)
    {
        len++;
        n /= 10;
    }
    for (i = 0; i < len; i++)
    {
        rem = num % 10;
        num = num / 10;
        str[len - (i + 1)] = rem + '0';
    }
    str[len] = '\0';
  
    return str;
}

char* char_to_string (char c) 
{ 
  static char str [2];
  str[0]=c; 
  str[1]='\0'; 
  return str; 
} 