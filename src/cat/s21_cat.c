#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "CatFlags.h"

typedef struct {
  bool NoEmpty;
  bool LastSymbols;
  bool PrintNonprintable;
  bool Number;
  bool Squeezyblank;
  bool Tabs;
} OurFlags;

OurFlags CatReadOurFlags(int argc, char *argv[]);
void CatFile(FILE *fp, OurFlags flags, const char *table[256]);
void OpenFile(int argc, char *argv[], OurFlags flags, const char *table[256]);

int main(int argc, char **argv) {
  const char *table[256];
  CatSetTable(table);
  OpenFile(argc, argv, CatReadOurFlags(argc, argv), table);
  return 0;
}

OurFlags CatReadOurFlags(int argc, char *argv[]) {
  struct option long_options[] = {
      {"number-nonblank", 0, 0, 'b'},
      {"number", 0, 0, 'n'},
      {"squeeze-blank", 0, 0, 's'},
      {0, 0, 0, 0},
  };
  int current_flag = getopt_long(argc, argv, "bevnstET", long_options, NULL);
  OurFlags flags = {false, false, false, false, false, false};
  for (; current_flag != -1;
       current_flag = getopt_long(argc, argv, "bevnstET", long_options, NULL)) {
    switch (current_flag) {
      case 'b':
        flags.NoEmpty = true;
        flags.Number = false;
        break;
      case 'e':
        flags.LastSymbols = true;
        flags.PrintNonprintable = true;
        break;
      case 'v':
        flags.PrintNonprintable = true;
        break;
      case 'n':
        flags.Number = true;
        break;
      case 's':
        flags.Squeezyblank = true;
        break;
      case 't':
        flags.Tabs = true;
        flags.PrintNonprintable = true;
        break;
      case 'E':
        flags.LastSymbols = true;
        break;
      case 'T':
        flags.Tabs = true;
        break;
    }
  }
  return flags;
}

void CatFile(FILE *fp, OurFlags flags, const char *table[256]) {
  int c = fgetc(fp);
  int last = '\n';
  int next = fgetc(fp);
  int line = 1;
  fseek(fp, -1, SEEK_CUR);
  if (flags.NoEmpty) {
    if (c != '\n') Number('\n', &line);
  } else if (flags.Number) {
    Number('\n', &line);
  }
  while (c != EOF) {
    if (flags.Squeezyblank) {
      if ((last == '\n' && c == '\n' && next == '\n')) {
        last = c;
        c = fgetc(fp);
        next = fgetc(fp);
        fseek(fp, -1, SEEK_CUR);
        continue;
      }
    }
    if (flags.LastSymbols) {
      LastSymbols(table);
    }
    if (flags.Tabs) {
      Tabs(table);
    }
    if (flags.PrintNonprintable) {
      PrintNonprintable(table);
    }
    if (!flags.PrintNonprintable && c == '\0') {
      printf("%c", c);
    } else {
      printf("%s", table[c]);
    }
    if (flags.NoEmpty && next != EOF) {
      if (next != '\n' && c == '\n') {
        Number(c, &line);
      }
    } else if (flags.Number) {
      Number(c, &line);
    }
    last = c;
    c = fgetc(fp);
    next = fgetc(fp);
    fseek(fp, -1, SEEK_CUR);
    if (next == EOF) {
      break;
    }
  }
  printf("%c", c);
}

void OpenFile(int argc, char *argv[], OurFlags flags, const char *table[256]) {
  for (char **filename = &argv[1], **end = &argv[argc]; filename != end;
       filename++) {
    if (**filename == '-') {
      continue;
    }
    FILE *fp = fopen(*filename, "r");
    if (fp != NULL) {
      CatFile(fp, flags, table);
    } else {
      fprintf(stderr, "Inncorect way");
    }
    fclose(fp);
  }
}
