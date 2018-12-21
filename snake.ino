#include <Arduboy2.h>

Arduboy2 arduboy;
unsigned int score = 0;
unsigned int moves = 1;
unsigned int speed = 5;
unsigned int state = 0;

int lastKey = 128; // defaults to up to
int tailX[50]; // max size of snake
int tailY[50]; // max size of snake

const int appleSize = 4;

Rect apple = {0, 0, appleSize, appleSize};
Rect snake = {WIDTH / 2, HEIGHT / 2, appleSize, appleSize};

void setup() {
  arduboy.begin(); 
  arduboy.setFrameRate(speed);
  randomizeApple();
}

void loop() {
  if (!(arduboy.nextFrame())) {
    return;
  }
  arduboy.pollButtons();
  arduboy.clear();
  manager();
  arduboy.display();
}

void gameRunner() {
  arduboy.setTextSize(1);
  checkButtons();
  arduboy.drawRoundRect(apple.x, apple.y, apple.width, apple.height, 1);
  moveSnake();
  arduboy.print("score: " + score);
}

void showHomeScreen() {
  arduboy.setTextSize(2);
  arduboy.setCursor(0, HEIGHT / 2);
  arduboy.println("perss A to continue");

  arduboy.print(state);
  if (arduboy.justPressed(A_BUTTON)) {
    state = 1;
  }
  arduboy.print(state);
}

void manager() {
  switch (state) {
    case 0:
        showHomeScreen();
      break;
    case 1:
        gameRunner();
      break;
      case 2:
        showGameOverScreen();
      break;
  }
}

void showGameOverScreen() {
  arduboy.setTextSize(2);
  arduboy.println("Gameover");
  arduboy.println("Score: " + score);

  if (arduboy.justPressed(A_BUTTON)) {
    state = 1;
  }
}

void checkButtons() {
  if (arduboy.justPressed(LEFT_BUTTON)) {
    if (lastKey == 64) return;
    lastKey = 32;
  }

  if (arduboy.justPressed(RIGHT_BUTTON)) {
    if (lastKey == 32) return;
    lastKey = 64;
  }

  if (arduboy.justPressed(UP_BUTTON)) {
    if (lastKey == 16) return;
    lastKey = 128;
  }

  if (arduboy.justPressed(DOWN_BUTTON)) {
    if (lastKey == 128) return;
    lastKey = 16;
  }
}

void moveSnake() {
  tailX[moves] = snake.x;
  tailY[moves] = snake.y;

  switch (lastKey) {
    case 32:
      snake.x -= appleSize;
    break;
    case 64:
        snake.x += appleSize;
    break;
    case 128:
      snake.y -= appleSize;
    break;
    case 16:
        snake.y += appleSize;
    break;
  }

  // array shift
  if (moves > 1) {
    memcpy(tailX, &tailX[1], sizeof(tailX) - sizeof(int));
    memcpy(tailY, &tailY[1], sizeof(tailY) - sizeof(int));
  }

  if (snake.x > WIDTH) {
    snake.x = 0;
  }

  if (snake.x < 0) {
    snake.x = WIDTH;
  }

  if (snake.y > HEIGHT) {
    snake.y = 0;
  }

  if (snake.y < 0) {
    snake.y = HEIGHT;
  }

  for (int i = 0; i <= moves; i++) {
      arduboy.drawRoundRect(tailX[i], tailY[i], snake.width, snake.height, 2, WHITE);
  }


  if (eatsApple()) {
    postEatEvents();
  }

  if (eatsItself()) {
      arduboy.println("dieeeeeee");
      moves = 0;
      state = 2;
  }
}

void randomizeApple() {
  apple.x = random(0, WIDTH);
  apple.y = random(0, HEIGHT);
}

bool eatsApple() {
  return arduboy.collide(snake, apple);
}

void postEatEvents() {
  randomizeApple();
  moves++;
  score += 10;
  // increase speed
  if (moves % 5 == 0) {
    speed++;
    arduboy.setFrameRate(speed);
  }
}

bool eatsItself() {
  for (int i = 0; i <= moves; i++) {
    // dont eat the head!
    if (i < moves - 1) {
      Rect currentTail =  {tailX[i], tailY[i], appleSize, appleSize};
      if (arduboy.collide(snake, currentTail)) {
        return true;
      }
    }
  }

  return false;
}
