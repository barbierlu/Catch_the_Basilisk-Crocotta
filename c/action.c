#include <stdio.h>
#include "action.h"
#include "board.h"

move get_player_move(void)
{
  //int rv, rv2;
  char c [30];
  printf("Enter your next move\n");
  if(!fgets(c, sizeof(c), stdin))
  {
    printf("fgets failed:/\n");
    return INVALID;
  }

  switch (c[0])
  {
    case 'a':
      return LEFT;
    case 'w':
      return UP;
    case 's':
      return DOWN;
    case 'd':
      return RIGHT;
    case 'h':
      return HELP;
    case 'x':
      // if (rv2 != 1)
      // {
      //   return INVALID;
      // }
      switch (c[1])
      {
        case 'a':
          return SHOOT_LEFT;
        case 'w':
          return SHOOT_UP;
        case 's':
          return SHOOT_DOWN;
        case 'd':
          return SHOOT_RIGHT;
        default:
          return INVALID;
      }
    default:
      return INVALID;
  }

}

void clear_stdin(void)
{
  int c;
  while( (c = getchar()) != '\n' && c!= EOF);
}

shot check_shot(board * brd, move m)
{
  int shot = brd->player_loc;
  int size = brd->size;
  switch(m)
  {
    case SHOOT_UP:
    shot -= size;
    break;
    case SHOOT_DOWN:
    shot += size;
    break;
    case SHOOT_RIGHT:
    shot += 1;
    break;
    default:
    shot -= 1;
  }
  if(check_occupied(brd, shot) == 1) // check if shot was in basilisk location
  {
    return HIT;
  }

  printf("Missed Shot!\n");
  return MISS;
}

void display_help(void)
{
  printf("\n\n\n\nTo move enter:\n");
  printf("a\tMove Left\nd\tMove Right\nw\tMove Up\ns\tMove Down\n");
  printf("Then hit enter\n");

  printf("\nTo shoot an arrow at the basilisk:\n");
  printf("Enter 'x' followed by a direction ['a','w','s','d']. (See above)\n");
  printf("Example: 'xd' would shoot right\n");
}

void display_intro(void)
{
  printf("Welcome to Catch the Basilisk!\n\n");
  printf("In this game you are a hunter in a dark cave, and there are 4 bad locations:\n\t1 Basilisk\n\t3 Pits\n");
  printf("-Your job is to move next to the Basilisk and shoot it with an arrow.\n");
  printf("-Moving is done with the [A]W][S]D] keys for [LEFT][UP][DOWN][RIGHT]\n");
  printf("-After each move hit ENTER\n\n");
  printf("-To shoot an arrow and kill basilisk enter 'x' followed by 'a', 'w', 's', or 'd'\n");
  printf("-At any time enter enter 'h' to see a list of options.\n");
  printf("-Missed shot arrows will move to a random location.\n");
  printf("Good Luck!\n");
}

void display_new_move(void)
{
  printf("\n\n***********************************************************\n");
}

void display_wall(void)
{
  printf("You ran into the wall of the cave!\n");
}

void display_unrecognized(void)
{
  printf("Command not recognized\n");
}

void display_win(void)
{
  printf("YOU KILLED THE BASILISK!\n");
  printf("YOU WON!\n");

}

void display_loss(death d)
{
  printf("OH NO YOU LOST!\n");
  if (d == PIT_D)
    printf("YOU STEPPED INTO A PIT!\n");
  else
    printf("YOU WERE EATEN BY THE BASILISK!\n");
}
