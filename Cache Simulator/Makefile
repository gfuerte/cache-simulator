all: simulator

simulator: simulator.c
	gcc -g -Wall -Werror -fsanitize=address -std=c11 simulaotr.c -o simulaot -lm

clean:
	rm -f simulator
