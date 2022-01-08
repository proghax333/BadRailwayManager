
#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define SIZE 64
#define SIZE_SM 32

#define CALLBACK

#define CREATE(type, size) ((type *)calloc(size, sizeof(type)))

typedef enum
{
  SUCCESS,
  FAILURE,
  EXIT
} ReturnCode;

char *set(char *dest, char *src)
{
  return strcpy(dest, src);
}

bool isEmpty(const char *str)
{
  if (!str)
    return true;

  return strlen(str) == 0;
}

bool equals(const char *a, const char *b)
{
  return a && b && strcmp(a, b) == 0;
}

typedef int (*TransformFunction)(int input);

void transform(char *str, size_t len, TransformFunction transformer)
{
  for (size_t i = 0; i < len; ++i)
  {
    str[i] = transformer(str[i]);
  }
}

bool equalsIgnoreCase(const char *a, const char *b)
{
  size_t lenA = strlen(a), lenB = strlen(b);
  char *strA = CREATE(char, lenA + 1);
  char *strB = CREATE(char, lenB + 1);

  strcpy(strA, a);
  strcpy(strB, b);

  transform(strA, lenA, toupper);
  transform(strB, lenB, toupper);

  bool result = strcmp(strA, strB) == 0;

  free(strA);
  free(strB);

  return result;
}

void getString(char *buf, size_t max)
{
  fgets(buf, max > 0 ? max : SIZE, stdin);
  int len = strlen(buf);
  buf[len - 1] = '\0';
}

int getInt()
{
  char buf[SIZE];
  getString(buf, SIZE);

  int value = strtol(buf, 0, 10);
  return value;
}

void clearScreen()
{
  system("clear");
}

void waitForKey()
{
  printf("\nPress any key to continue...\n");
  getInt();
}
