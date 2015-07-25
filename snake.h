#define WORLD_WIDTH 50
#define WORLD_HEIGHT 20
 
#define LENGTH 41
#define BODY 100000
#define DOLLER 2
#define RARE 4
#define SRARE 15

static int length = LENGTH;
static int flag;
static int bonus;
static int sbonus;
static int score;
enum direction { UP, DOWN, RIGHT, LEFT };

typedef struct snake {
    int x;
    int y;
} SNAKE;

typedef struct seed {
	int x;
	int y;
} SEED;

void game_over(WINDOW *);
void put_seed(WINDOW *, SNAKE [], SEED [], int, int);
void move_snake(WINDOW *, SNAKE [], SEED [], int);
