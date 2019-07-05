#include <stdio.h>
#include "board.h"
#include "action.h"

#define BOARD_SIZE (5)

int main (void)
{
  display_intro();

  printf("\nLoading Board...\n");
  board * brd = init_board(BOARD_SIZE);    // initialize board of size 5*5v
    if(!brd)
    {
      printf("Board failed to initialize.\n");
      return -1;
    }
  arrows * arr = init_arrows();   // initialize arrow objects
  if(!arr)
  {
    printf("Arrows failed to initialize.\n");
    delete_board(brd);
    return -2;
  }

  int alive = 1, m, d;
  while(alive) // main game loop
  {
    display_new_move();
    printf("Current Location: %d\n", brd->player_loc);
    printf("Arrow count: %d\n", arr->user_rem);
    update_board(brd);
    print_board(brd, arr);
    m = get_player_move();
    pick_up_arrow(brd, arr);

    if(m == INVALID)
    {
      display_unrecognized();
      display_help();
    }
    else if (m == HELP)
    {
      display_help();
    }
    else if (m >= SHOOT_UP)
    {
      if(arr->user_rem < 1)
        {
          printf("You're out of arrows!\n");
          continue;
        }
      if (!(check_shot(brd, m)))
      {
        display_win();
        break;
      }
      else
      {
        place_arrow(brd, arr);
        arr->user_rem--;
      }

    }
    else    // player movement
    {
      switch (check_movement(brd, m))
      {
        case OUT_OF_BOUNDS:
          display_wall();
          break;
        case PIT_D:
          display_loss(PIT_D);
          alive = 0;
          break;
        case BASILISK_D:
          display_loss(BASILISK_D);
          alive = 0;
          break;
        default: // safe
          break;
      }
    }
  }

  end_game_show_locs(brd, arr);
  display_new_move();
  print_board(brd, arr);

  delete_board(brd);
  delete_arrow(arr);
  return 0;
}
