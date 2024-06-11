// #include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <curses.h>

int running = 1;

void exit_program_handler(int n);

void main_program(void) {
}

int main() {

	int num = 0;
	signal(SIGINT, exit_program_handler);

	initscr();
	keypad(stdscr, TRUE);
	nonl();
	cbreak();
	echo();

	if (has_colors()) {
		start_color();

		init_pair(1, COLOR_RED, COLOR_BLACK);
		init_pair(2, COLOR_GREEN, COLOR_BLACK);
		init_pair(3, COLOR_YELLOW, COLOR_BLACK);
		init_pair(4, COLOR_BLUE, COLOR_BLACK);
		init_pair(5, COLOR_CYAN, COLOR_BLACK);
		init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
		init_pair(7, COLOR_WHITE, COLOR_BLACK);
	}

	for (;;) {
		// int c = getch();
		// wgetch(stdscr);
		attrset(COLOR_PAIR(num % 8));
		printw("A");
		refresh();
		num++;
	}

	exit_program_handler(0);

	// while (running) {
	// 	main_program();
	// }
	// return 0;
}

void exit_program_handler(int n) {
	running = 0;
	endwin();
	exit(0);
}
