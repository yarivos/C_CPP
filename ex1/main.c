#include "cipher.h"
#include "tests.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define MAX_COUNT 1024
#define ARGS_TEST 2
#define ARGS_CODE 5
#define BASE_10 10

// your code goes here
int check_given_shift (char k[]);

int checking_args (int argc);
int test_cipher ();

int main (int argc, char *argv[])
{


  FILE *fptr1, *fptr2;
  char buffer[MAX_COUNT];
  if (checking_args (argc))
  {
    fprintf (stderr, "The program receives 1 or 4 arguments only.\n");
    return EXIT_FAILURE;
  }
  if (argc == ARGS_TEST) //test
  {
    if (!(strcmp (argv[1], "test")))
    {
      // do test
      if (test_cipher ())
      {
        return EXIT_FAILURE;
      }
      return EXIT_SUCCESS;
    }
    else //Error in the command
    {
      fprintf (stderr, "Usage: cipher test\n");
      return EXIT_FAILURE;
    }
  }
  if ((argc == ARGS_CODE)
      && (!(strcmp (argv[1], "encode")) || !(strcmp (argv[1], "decode"))))
  {
    if (!(strcmp (argv[1], "encode")))
    {      // do encode
      if (check_given_shift (argv[2]))
      {
        fprintf (stderr, "The given shift value is invalid.\n");
        return EXIT_FAILURE;
      }
      fptr1 = fopen (argv[3], "r");
      if (fptr1 == NULL) // file is invalid
      {
        fprintf (stderr, "The given file is invalid.\n");
        return EXIT_FAILURE;
      }
      fptr2 = fopen (argv[4], (const char *) "w");
      if (fptr2 == NULL)
      {
        fprintf (stderr, "The given file is invalid.\n");
        fclose (fptr1);
        return EXIT_FAILURE;
      }
      while (fgets (buffer, MAX_COUNT, fptr1))
      {
        encode (buffer, strtol (argv[2], NULL, BASE_10));
        fprintf (fptr2, "%s", buffer);
      }
      fclose (fptr1);
      fclose (fptr2);
      return EXIT_SUCCESS;
    }
    else if (!(strcmp (argv[1], "decode")))
    {     // do decode
      if (check_given_shift (argv[2]))
      {
        fprintf (stderr, "The given shift value is invalid.\n");
        return EXIT_FAILURE;
      }
      fptr1 = fopen (argv[3], "r");
      if (fptr1 == NULL) // file is invalid
      {
        fprintf (stderr, "The given file is invalid.\n");
        return EXIT_FAILURE;
      }
      fptr2 = fopen (argv[4], (const char *) "w");
      if (fptr2 == NULL)
      {
        fprintf (stderr, "The given file is invalid.\n");
        fclose (fptr1);
        return EXIT_FAILURE;
      }
      while (fgets (buffer, MAX_COUNT, fptr1))
      {
        encode (buffer, -(strtol (argv[2], NULL, BASE_10)));
        fprintf (fptr2, "%s", buffer);
      }
      fclose (fptr1);
      fclose (fptr2);
      return EXIT_SUCCESS;
    }
  }
  else //Error in the command
  {
    fprintf (stderr, "The given command is invalid.\n");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int checking_args (int argc)
{
  if (argc != ARGS_CODE && argc != ARGS_TEST)
  {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int check_given_shift (char k[])
{
  if (k[0] != '-' && !isdigit (k[0]))
  {
    return EXIT_FAILURE;
  }
  for (int i = 1; k[i] != '\0'; i++)
  {
    if (!isdigit (k[i]))
    {
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}

int test_cipher ()
{
  if (test_encode_non_cyclic_lower_case_positive_k ())
  {
    return EXIT_FAILURE;
  }
  if (test_encode_cyclic_lower_case_special_char_positive_k ())
  {
    return EXIT_FAILURE;
  }
  if (test_encode_non_cyclic_lower_case_special_char_negative_k ())
  {
    return EXIT_FAILURE;
  }
  if (test_encode_cyclic_lower_case_negative_k ())
  {
    return EXIT_FAILURE;
  }
  if (test_encode_cyclic_upper_case_positive_k ())
  {
    return EXIT_FAILURE;
  }
  if (test_decode_non_cyclic_lower_case_positive_k ())
  {
    return EXIT_FAILURE;
  }
  if (test_decode_cyclic_lower_case_special_char_positive_k ())
  {
    return EXIT_FAILURE;
  }
  if (test_decode_non_cyclic_lower_case_special_char_negative_k ())
  {
    return EXIT_FAILURE;
  }
  if (test_decode_cyclic_lower_case_negative_k ())
  {
    return EXIT_FAILURE;
  }
  if (test_decode_cyclic_upper_case_positive_k ())
  {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}