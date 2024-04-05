CC = gcc -Wall -Werror -Wextra
SRC = s21_grep.c
SRS = ../cat/s21_cat.c ../cat/CatFlags.c

s21_grep:
	$(CC) -c $(SRC)
	$(CC) *.o -o s21_grep
	rm -rf *.o

s21_cat:
	$(CC) -c $(SRS)
	$(CC) *.o -o ../cat/s21_cat
	rm -rf *.o

clean:
	rm -rf *.o s21_grep
	rm -rf *.o ../cat/s21_cat