#ifndef CATFLAGS_H
#define CATFLAGS_H

void CatSetTable(const char *table[256]);
void LastSymbols(const char *table[256]);
void Tabs(const char *table[256]);
void PrintNonprintable(const char *table[256]);
void Number(int c, int *line);
#endif