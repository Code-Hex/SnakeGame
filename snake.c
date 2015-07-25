#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <limits.h>
#include "snake.h"

int main() {
	WINDOW *win;
	int i, j, x, y, c, result_x, result_y, dir;
	int start_x = 0, start_y = 1; 
	int score = 0;
	SNAKE snake[BODY];
	SEED seed[3];
	
	initscr();
	start_color();
    init_pair(1, COLOR_WHITE, COLOR_RED);
    init_pair(2, COLOR_WHITE, COLOR_GREEN);
    init_pair(3, COLOR_WHITE, COLOR_YELLOW);
    init_pair(4, COLOR_WHITE,  COLOR_BLUE);
    init_pair(5, COLOR_WHITE, COLOR_MAGENTA);
    init_pair(6, COLOR_WHITE, COLOR_CYAN);
    cbreak();
    timeout(100);
    keypad(stdscr, TRUE);

	do {
		clear();
		win = newwin(LINES - 1, COLS, start_y, start_x);
		length = LENGTH;
		flag = 0; score = 0;
		for (i = 0; i < length; ++i) {
			snake[i].x = i + start_x;
			snake[i].y = start_y;
		}

		dir = RIGHT;
		put_seed(win, snake, seed, '$', 0);
		printw("Exit at \'q\', Score: %d\r", score);
	    refresh();
		while ((c = getch()) != 'q') {
			if (j < INT_MAX) j += 1;
			else j = 0;

			move_snake(win, snake, seed, dir);
			if (snake[length - 1].x == seed[0].x && snake[length - 1].y == seed[0].y) {
				length += 2;
				snake[length - 1] = snake[length - 3]; // + length
				snake[length - 2] = snake[length - 4]; // + length
				put_seed(win, snake, seed, '$', 0);
				move_snake(win, snake, seed, dir);
				score += 100;
			} else if (snake[length - 1].x == seed[1].x && snake[length - 1].y == seed[1].y) {
				bonus = 0;
				length += 4;
				for (i = 1; i < 5; i++)
					snake[length - i] = snake[length - (i + 4)]; // + length
				move_snake(win, snake, seed, dir);
				score += 500;
			} else if (j % 100 == 0 && bonus == 0) {
				bonus = 1;
				put_seed(win, snake, seed, '*' | COLOR_PAIR(3), 1);
			} else if (flag) break;

			printw("Exit at \'q\', Score: %d\r", score);
	    	refresh();

			if (c != ERR) {
				switch (c) {
					case KEY_UP:
						dir = UP;
						break;
					case KEY_DOWN:
						dir = DOWN;
						break;
					case KEY_RIGHT:
						dir = RIGHT;
						break;
					case KEY_LEFT:
						dir = LEFT;
						break;
					default:
						break;
				}
			}
		}
		game_over(win);
	} while ((c = getchar()) != 'q');
	delwin(win);
	endwin();
    return 0;
}

void game_over(WINDOW *win){
	clear();
	wclear(win);
	printw("*** GameOver ***\n");
	printw("Exit at \'q\'\n");
	printw("Enter of space key to continue\n");
    wrefresh(win);
    refresh();
}

void put_seed(WINDOW *win, SNAKE snake[], SEED seed[], int item, int n){
	srand((unsigned) time(NULL));
	int i, x, y, found;
	do { 
		x = (rand() % (COLS - 2)) + 1;
		y = (rand() % (LINES - 3)) + 1;

		// snake search!!
		for (i = 0; i < length; i++) {
			if (snake[i].x == x && snake[i].y == y) {
				found = 0;
				break;
			} else found = 1;
		}
	} while (!found);
	seed[n].x = x;
	seed[n].y = y;

	mvwaddch(win, seed[n].y, seed[n].x, item);
	wrefresh(win);
}

void move_snake(WINDOW *win, SNAKE snake[], SEED seed[], int dir){
	wclear(win);
	int i, j;
	srand((unsigned) time(NULL));
	
	
	for (i = 0; i < length - 1; i++) {
        snake[i] = snake[i + 1];
        mvwaddch(win, snake[i].y, snake[i].x, ' ' | COLOR_PAIR((rand() % 6) + 1));
    }

    int x = snake[length - 1].x;
    int y = snake[length - 1].y;

    switch (dir) {
		case UP:
			y <= 1 ? flag = 1 : y--;
			break;
		case DOWN:
			(LINES - 3) <= y ? flag = 1 : y++;
			break;
		case RIGHT:
			(COLS - 2) <= x ? flag = 1 : x++;
			break;
		case LEFT:
			x <= 1 ? flag = 1 : x--;
			break;
		default:
			break;
	}

	//check snake head to body
	if (!flag) {
		for (j = 0; j < length - 1; j++) {
			if (snake[j].x == x && snake[j].y == y) {
				flag = 1;
				break;
			}
		}
	}

 	mvwaddch(win, seed[0].y, seed[0].x, '$'); // keep a live money
 	if (bonus)
		mvwaddch(win, seed[1].y, seed[1].x, '*' | COLOR_PAIR(3)); // keep a live money

	mvwaddch(win, y, x, ' ' | COLOR_PAIR((rand() % 6) + 1));

	snake[length - 1].x = x;
	snake[length - 1].y = y;
	box(win, 0, 0);
	wrefresh(win);
}