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
  }
  else if(x < 1 || x > ROZMIAR || y < 1 || y > (ROZMIAR*2))
  {
    quit = true;
  }
}

void snake_move(Snake *wunsz,int x, int y)
{
  int o_x, o_y;
  Snake *iter = wunsz->next;
  mvaddstr(wunsz->posx, wunsz->posy, "0");
  collision(wunsz->posx,wunsz->posy);
  o_x = wunsz->posx;
  o_y = wunsz->posy;
  // while (iter != NULL)
  // {
  //   iter->posx 
  // }
  
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
  Snake wunsz;
  wunsz.posx = 10;
  wunsz.posy = 10;
  do {
    int c = getch();
    switch (c) 
    {
    case 'q':
        quit = true;
        break;
	case 'w':
		x = -1;
		y = 0;
		break;

	case 'a':
		x = 0;
		y = -1;
		break;

	case 'd':
		x = 0;
		y = 1;
		break;
	case 's':
		x = 1;
		y = 0;
		break;

      default:
        break;
    }

  clear();
  map();
  food();
  snake_move(&wunsz,x,y);
  mvaddstr(31,0,"score:");
  mvaddstr(32,0,"Press q to exit"); 
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