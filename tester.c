#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>

int main(void) 
{
  WINDOW * mainwin;

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
  int posx = 10;
  int posy = 0;
  bool quit = false;
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
    mvaddstr(posx, posy, "Tu pelznie waz (nacisnij 'q' by zakonczyc)");
    posx += x;
	posy += y;
	refresh();
  } while( ! quit );


  // Czyszczenie
  nocbreak();
  echo();
  refresh();
  delwin(mainwin);
  endwin();
  return EXIT_SUCCESS;
}