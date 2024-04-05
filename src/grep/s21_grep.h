#ifndef S21_GREP_H
#define S21_GREP_H

#include <getopt.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  bool Template;
  bool NoRegister;
  bool InvertSearch;
  bool OnlyNumbers;
  bool OnlyFiles;
  bool PredvorStrok;
  bool Sovbodstroki;
  bool NoErrors;
  bool RegfromFile;
  bool Oflags;
} OurFlags;

typedef struct {
  char **files;
  char *express;
  int countfiles;
  int countepress;
  OurFlags flags;
} OurAgruments;

OurAgruments *GrepFlags(int agrc, char *argv[]);
void OpenFile(OurAgruments *arguments, int argc);
void Regexfile(FILE *fp, OurFlags flags, char *filename, char *express,
               int countOfFiles);
void Fflags(char *optarg, OurAgruments *arguments);
void Match(regex_t expression, char *string);
void Free(OurAgruments *arguments, int argc);

#endif