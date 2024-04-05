CC = gcc -Wall -Werror -Wextra
SRC = s21_cat.c CatFlags.c

s21_cat:
	$(CC) -c $(SRC)
	$(CC) *.o -o s21_cat
	rm -rf *.o

clean:
	rm -rf *.o s21_cat