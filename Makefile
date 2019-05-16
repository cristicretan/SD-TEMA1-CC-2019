build: tema.c
	gcc -Wall -g -std=c11  tema.c -o tema1 -lm
run: tema
	./tema1
clean:
	rm tema1
