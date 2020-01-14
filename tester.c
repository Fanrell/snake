#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include <string.h>
#include <time.h>
#define ROZMIAR 30

typedef struct snake
{
  int posx;
  int posy;
  struct snake *next;
}Snake;
Snake *wunsz = NULL;
Snake *nowy (int x, int y)
{
  Snake *nowy;
  if((nowy = (Snake*)malloc(sizeof(Snake))) == NULL)
  {
    exit(-1);
  }
  nowy->posx = x;
  nowy->posy = y;
  nowy->next = NULL;
  return nowy;
}

void growth()
{
  Snake *iter = wunsz;
  while (iter->next != NULL)
  {
    iter = iter->next;
  }
  iter->next = nowy(50,50);
}

int food_x = -1024;
int food_y = -1024;
int score = 0;
bool quit = false;
WINDOW * mainwin;

void map()
{
  for(int i=0;i<=ROZMIAR;i++)
  {
    mvaddstr(i,0,"|");
    mvaddstr(i,ROZMIAR*2,"|");
  }
  for(int i=0;i<=ROZMIAR*2;i++)
  {
    mvaddstr(0,i,"-");
    mvaddstr(ROZMIAR,i,"-");
  }
}

void food()
{
  int seed;
  time_t tt;
  seed = time(&tt);
  if(food_x == -1024 && food_y == -1024)
  {
    srand(seed);
    food_x = rand()%ROZMIAR;
    if(food_x == 0) food_x+=1;
    if(food_x == 0) food_x+=1;
    srand(seed);
    food_y = rand()%(ROZMIAR*2);
    if(food_y == 0) food_y+=1;
    if(food_y == (ROZMIAR*2)) food_y-=1;
  }
  mvaddstr(food_x,food_y,"*");
}

void collision(const int x,const int y)
{
  if(x == food_x && y == food_y)
  {
      score+=10;
      food_x = -1024;
      food_y = -1024;
      growth();
  }
  else if(x < 1 || x > ROZMIAR || y < 1 || y > (ROZMIAR*2))
  {
    quit = true;
  }
  Snake *iter = wunsz->next;

  while (iter != NULL)
  {
    if(wunsz->posx == iter->posx && wunsz->posy == iter->posy)
    {
      quit = true;
      break;
    }
    iter = iter->next;
  }
  
}

void mover(Snake *wunsz)
{
  mvaddstr(wunsz->posx, wunsz->posy, "0");
  if(wunsz->next != NULL)
  {
    mover(wunsz->next);
  }
}

void snake_move(Snake *wunsz,int x, int y)
{
  int o_x, o_y;
  Snake *iter = wunsz;

  mover(wunsz);

  collision(wunsz->posx,wunsz->posy);
  int i = 0;
  while (iter != NULL)
  {
    int tx, ty;
    if(i == 0)
    {
      o_x = iter->posx;
      o_y = iter->posy;
    }
    if(i>=1)
    {
      tx = iter->posx;
      ty = iter->posy;
      iter->posx = o_x;
      iter->posy = o_y;
      o_x = tx;
      o_y = ty;
    }
    iter = iter->next;
    i++;
    
  }
  
  
  wunsz->posx += x;
	wunsz->posy += y;

}

int main(void) 
{

  // Initializacja ncurses
  if ( (mainwin = initscr()) == NULL ) {
    fprintf(stderr, "Error initialising ncurses.\n");
    exit(EXIT_FAILURE);
  }
  noecho();         // nie pokazuj wpisywanych danych
  cbreak();         // disables line buffering and erase/kill character-processing
  timeout(500);     // wait 500ms for key press

  // Tak można dac X we wskazanym miejscu lub uzyc mvaddstr jak nizej
  //move(0, 0); 
  //addch('X');

  int x,y;
  x = 0;
  y = 1;
  wunsz = nowy(10,10);
  bool w,s,a,d;
  w = true;
  s = true;
  a = false;
  keypad(stdscr, TRUE);
  do {
    int c = getch();
    switch (c) 
    {
    case 'q':
        quit = true;
        break;
	case KEY_UP:
  if(w)
  {
		x = -1;
		y = 0;
    a = true;
    d = true;
    s = false;
  }
		break;

	case KEY_LEFT:
  if(a)
  {
		x = 0;
		y = -1;
    s = true;
    w = true;
    d = false;
  }
		break;

	case KEY_RIGHT:
  if(d)
  {
		x = 0;
		y = 1;
    s = true;
    w = true;
    a = false;
  }
		break;
	case KEY_DOWN:
  if(s)
  {
		x = 1;
		y = 0;
    a = true;
    d = true;
    w = false;
  }
		break;

      default:
        break;
    }

  clear();
  map();
  food();
  snake_move(wunsz,x,y);
  mvprintw(31,0,"score: %d", score);
  mvprintw(32,0,"controles: arrows keys");
  mvprintw(33,0,"Press q to exit"); 
	refresh();
  
  } while( ! quit );


  // Czyszczenie
  nocbreak();
  echo();
  refresh();
  delwin(mainwin);
  endwin();
  printf("GAME OVER\nScore: %d\n",score);
  return EXIT_SUCCESS;
}