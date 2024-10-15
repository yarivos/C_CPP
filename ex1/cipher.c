#include "cipher.h"
#define HIGH_CASE_BOTTOM 65
#define HIGH_CASE_TOP 90
#define LOWER_CASE_BOTTOM 97
#define LOWER_CASE_TOP 122
#define HIGH_CASE_CHAR_BOTTOM 'A'
#define HIGH_CASE_CHAR_TOP 'Z'
#define LOWER_CASE_CHAR_BOTTOM 'a'
#define LOWER_CASE_CHAR_TOP 'z'
/// IN THIS FILE, IMPLEMENT EVERY FUNCTION THAT'S DECLARED IN cipher.h.
/// Higher case A= 65 Z=90
/// Lower case a= 97 z= 122
char shift_char_in_range (char c, int k, char first_char, char last_char);
// See full documentation in header file
void encode (char s[], int k)
{
  for (int i = 0; s[i] != '\0'; i++)
  {
    if (s[i] >= HIGH_CASE_BOTTOM && s[i] <= HIGH_CASE_TOP) // High Case Letter
    {
      s[i] = shift_char_in_range (s[i], k, HIGH_CASE_CHAR_BOTTOM,
                                  HIGH_CASE_CHAR_TOP);
    }
    if (s[i] >= LOWER_CASE_BOTTOM\
 && s[i] <= LOWER_CASE_TOP) // Lower Case Letter
    {
      s[i] = shift_char_in_range (s[i], k, LOWER_CASE_CHAR_BOTTOM,
                                  LOWER_CASE_CHAR_TOP);
    }
  }

}

// See full documentation in header file
void decode (char s[], int k)
{
  // your code goes here
  for (int i = 0; s[i] != '\0'; i++)
  {
    if (s[i] >= HIGH_CASE_BOTTOM && s[i] <= HIGH_CASE_TOP) // High Case Letter
    {
      s[i] = shift_char_in_range (s[i], -k,
                                  HIGH_CASE_CHAR_BOTTOM,
                                  HIGH_CASE_CHAR_TOP);
    }
    if (s[i] >= LOWER_CASE_BOTTOM\
 && s[i] <= LOWER_CASE_TOP) // Lower Case Letter
    {
      s[i] = shift_char_in_range (s[i], -k,
                                  LOWER_CASE_CHAR_BOTTOM,
                                  LOWER_CASE_CHAR_TOP);
    }
  }

}
char shift_char_in_range (char c, int k, char first_char, char last_char)
{
  if ((k == 0) || (c < first_char) || (c > last_char))
  {
    return c;
  }
  if (k > 0)
  {
    int shift = k % (last_char - first_char + 1);
    for (int i = 0; i < shift; i++)
    {
      if (c == last_char)
      {
        c = first_char;
      }
      else
      {
        c++;
      }
    }
  }
  if (k < 0)
  {
    int shift = (-k) % (last_char - first_char + 1);
    for (int i = 0; i < shift; i++)
    {
      if (c == first_char)
      {
        c = last_char;
      }
      else
      {
        c--;
      }
    }
  }
  return c;
}


