#include "s21_grep.h"

int main(int argc, char **argv) {
  OpenFile(GrepFlags(argc, argv), argc);
  return 0;
}

OurAgruments *GrepFlags(int argc, char *argv[]) {
  int current_flags = 0;
  OurFlags flags = {false, false, false, false, false,
                    false, false, false, false, false};
  OurAgruments *arguments = calloc(1, sizeof(OurAgruments));
  arguments->countfiles = 0;
  arguments->countepress = 0;
  arguments->flags = flags;
  arguments->express = (char *)calloc(1024, sizeof(char));
  arguments->files = (char **)calloc(100, sizeof(char));
  for (int i = 0; i < argc; i++) {
    arguments->files[i] = (char *)calloc(1024, sizeof(char));
  }
  while (current_flags != -1) {
    current_flags = getopt_long(argc, argv, "e:ivclnhsf:o", NULL, 0);
    switch (current_flags) {
      case 'e':
        arguments->flags.Template = true;
        strcat(arguments->express, optarg);
        arguments->countepress++;
        strcat(arguments->express, "|");
        break;
      case 'i':
        arguments->flags.NoRegister = true;
        break;
      case 'v':
        arguments->flags.InvertSearch = true;
        break;
      case 'c':
        arguments->flags.OnlyNumbers = true;
        break;
      case 'l':
        arguments->flags.OnlyFiles = true;
        break;
      case 'n':
        arguments->flags.PredvorStrok = true;
        break;
      case 'h':
        arguments->flags.Sovbodstroki = true;
        break;
      case 's':
        arguments->flags.NoErrors = true;
        break;
      case 'f':
        arguments->flags.RegfromFile = true;
        Fflags(argv[optind - 1], arguments);
        break;
      case 'o':
        arguments->flags.Oflags = true;
        break;
      default:
        if (arguments->flags.Template == false) {
          if (arguments->flags.RegfromFile != true) {
            if (strchr(arguments->express, 'e') == NULL) {
              strcat(arguments->express, argv[optind]);
              strcat(arguments->express, "|");
              arguments->countepress++;
              optind++;
            }
          }
        }
        strcat(arguments->files[arguments->countfiles++], argv[optind]);
        optind++;
    }
  }
  if (arguments->flags.RegfromFile != true) {
    arguments->express[strlen(arguments->express) - 1] = '\0';
  }
  return arguments;
}

void OpenFile(OurAgruments *arguments, int agrc) {
  for (int i = 0; i < arguments->countfiles; i++) {
    FILE *fp = fopen(arguments->files[i], "r");
    if (fp != NULL) {
      Regexfile(fp, arguments->flags, arguments->files[i], arguments->express,
                arguments->countfiles);
      fclose(fp);
    } else if (fp == NULL && arguments->flags.NoErrors != true) {
      fprintf(stderr, "No such file or directory\n");
    }
  }
  Free(arguments, agrc);
}

void Regexfile(FILE *fp, OurFlags flags, char *filename, char *express,
               int countOfFiles) {
  regex_t reegex;
  int line = 0;
  int count = 0;
  regcomp(&reegex, express,
          REG_EXTENDED | (flags.NoRegister ? REG_ICASE : 0000));
  char *s = calloc(1024, sizeof(char));
  while (!feof(fp)) {
    line++;
    if (fgets(s, 1024, fp)) {
      if ((regexec(&reegex, s, 0, NULL, 0)) == flags.InvertSearch) {
        count++;
        if (flags.OnlyNumbers != true && flags.OnlyFiles != true) {
          if (!(flags.Sovbodstroki) && countOfFiles > 1) {
            printf("%s:", filename);
          }
          if (flags.PredvorStrok) {
            printf("%d:", line);
          }
          if (flags.Oflags != true) {
            if (s[strlen(s) - 1] == '\n') {
              printf("%s", s);
            } else {
              printf("%s\n", s);
            }
          }
        }
      }
    }
    if (flags.RegfromFile != true && flags.Oflags == true) {
      Match(reegex, s);
    }
    s[0] = '\0';
  }
  regfree(&reegex);
  if (flags.OnlyNumbers && (flags.OnlyFiles)) {
    if (!(flags.Sovbodstroki) && countOfFiles > 1) {
      printf("%s:", filename);
    }
    if (count == 0) {
      printf("0\n");
    } else {
      printf("1\n");
    }
  }
  if (flags.OnlyNumbers && (flags.OnlyFiles != true)) {
    if (!(flags.Sovbodstroki) && countOfFiles > 1) {
      printf("%s:", filename);
    }
    printf("%d\n", count);
  }
  if (flags.OnlyFiles && count != 0) {
    printf("%s\n", filename);
  }
  free(s);
}

void Fflags(char *optarg, OurAgruments *arguments) {
  FILE *fp = fopen(optarg, "r");
  if (fp != NULL) {
    char *s = calloc(1024, sizeof(char));
    while (!feof(fp)) {
      if (fgets(s, 1024, fp)) {
        strcat(arguments->express, s);
        strcat(arguments->express, "|");
      }
    }
    arguments->express[strlen(arguments->express) - 1] = '\0';
    fclose(fp);
    free(s);
  } else {
    fprintf(stderr, "NOFILE");
  }
}

void Match(regex_t expression, char *string) {
  regmatch_t matches[2];
  char *tempString = string;
  while (!regexec(&expression, tempString, 2, matches, 0)) {
    for (int i = matches[0].rm_so; i < matches[0].rm_eo; i++) {
      printf("%c", tempString[i]);
    }
    int count = 0;
    while (count != matches[0].rm_eo) {
      tempString++;
      count++;
    }
    printf("\n");
  }
}

void Free(OurAgruments *arguments, int argc) {
  for (int i = 0; i < argc; i++) {
    if (arguments->files[i] != NULL) free(arguments->files[i]);
  }
  if (arguments->files != NULL) {
    free(arguments->files);
  }
  if ((arguments->express) != NULL) {
    free(arguments->express);
  }
  if (arguments != NULL) {
    free(arguments);
  }
}
