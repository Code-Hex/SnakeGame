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
    timeout(130);
    keypad(stdscr, TRUE);

	do {
		clear();
		win = newwin(LINES - 1, COLS, start_y, start_x);
		length = LENGTH;
		flag = 0; score = 0; j = 0;
		for (i = 0; i < length; ++i) {
			snake[i].x = i + start_x;
			snake[i].y = start_y;
		}

		dir = RIGHT;
		put_seed(win, snake, seed, '$', 0);
		printw("Exit with \'q\', Score: %d\r", score);
	    refresh();
		while ((c = getch()) != 'q') {
			if (j < INT_MAX) j += 1;
			else j = 0;

			if (score >= 500 && score < 5000) timeout(100);
			else if (score >= 5000 && score < 10000) timeout(80);
			else if (score >= 10000 && score < 25000) timeout(60);
			else if (score >= 25000) timeout(50);

			move_snake(win, snake, seed, dir);
			// when snake get seed $
			if (snake[length - 1].x == seed[0].x && snake[length - 1].y == seed[0].y) {
				length += DOLLER;
				for (i = 1; i <= DOLLER; i++)
					snake[length - i] = snake[length - (i + DOLLER)]; // + length
				put_seed(win, snake, seed, '$', 0);
				move_snake(win, snake, seed, dir);
				score += 100;

			// when snake get seed *
			} else if (snake[length - 1].x == seed[1].x && snake[length - 1].y == seed[1].y) {
				bonus = 0;
				length += RARE;
				for (i = 1; i <= RARE; i++)
					snake[length - i] = snake[length - (i + RARE)]; // + length
				move_snake(win, snake, seed, dir);
				score += 500;

			// when snake get seed ?
			} else if (snake[length - 1].x == seed[2].x && snake[length - 1].y == seed[2].y) {
				sbonus = 0;
				length += SRARE;
				for (i = 1; i <= SRARE; i++)
					snake[length - i] = snake[length - (i + SRARE)]; // + length
				move_snake(win, snake, seed, dir);
				score += 2000;

			// produce rare item(*)
			} else if (j % 100 == 0 && bonus == 0) {
				bonus = 1;
				put_seed(win, snake, seed, '*' | COLOR_PAIR(3), 1);

			// produce super rare item(*)
			} else if (j % 600 == 0 && sbonus == 0) {
				sbonus = 1;
				put_seed(win, snake, seed, '?' | COLOR_PAIR(4), 2);
			} else if (flag) break;

			printw("Exit with \'q\', Score: %d\r", score);
	    	refresh();

			if (c != ERR) {
				switch (c) {
					case KEY_UP:
						if (dir != DOWN) dir = UP;
						break;
					case KEY_DOWN:
						if (dir != UP) dir = DOWN;
						break;
					case KEY_RIGHT:
						if (dir != LEFT) dir = RIGHT;
						break;
					case KEY_LEFT:
						if (dir != RIGHT) dir = LEFT;
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
	if (score >= 25000)
		printw("YOU'RE GREAT CHALLENGER!!\n");
	else
		printw("*** GameOver ***\n");
	printw("Exit with \'q\', Score: %d\n", score);
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
		/*
		for (i = 0; i < length; i++) {
			if (snake[i].x == x && snake[i].y == y) {
				found = 0;
				break;
			} else found = 1;
		}
		*/
		// search only head
		if (snake[0].x == x && snake[0].y == y) {
			found = 0;
			break;
		} else found = 1;

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

	if (sbonus)
		mvwaddch(win, seed[2].y, seed[2].x, '?' | COLOR_PAIR(4)); // keep a live money

	mvwaddch(win, y, x, ' ' | COLOR_PAIR((rand() % 6) + 1)); // draw a head

	snake[length - 1].x = x;
	snake[length - 1].y = y;
	box(win, 0, 0);
	wrefresh(win);
}