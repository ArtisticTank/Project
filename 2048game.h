#define SIZE 4

int score = 0;

void print(int tiles[SIZE][SIZE]);
int findTarget(int array[SIZE], int x, int stop);
int merge(int array[SIZE]);
void rotatetiles(int tiles[SIZE][SIZE]);
int moveUp(int tiles[SIZE][SIZE]);
int moveLeft(int tiles[SIZE][SIZE]);
int moveDown(int tiles[SIZE][SIZE]);
int moveRight(int tiles[SIZE][SIZE]);
int findPairDown(int tiles[SIZE][SIZE]);
int countEmpty(int tiles[SIZE][SIZE]);
int gameEnded(int tiles[SIZE][SIZE]);
void addRandom(int tiles[SIZE][SIZE]);
void inittiles(int tiles[SIZE][SIZE]);
