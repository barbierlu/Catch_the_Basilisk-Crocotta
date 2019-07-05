#include "board.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#define MIN_BRD_SIZE      4
#define ARROW_START_NUM   2
#define PIT_NUM           3


arrows * init_arrows(void)
{
  arrows * arr = malloc(sizeof(arrows));
    if(!arr) return NULL;
  arr->user_rem = ARROW_START_NUM;
  arr->locs = malloc(ARROW_START_NUM * sizeof(int));
    if(!arr->locs)
    {
      free(arr);
      return NULL;
    }
  for (int i = 0; i < ARROW_START_NUM; i++)
  {
    arr->locs[i] == -1;
  }
  return arr;
}


board * init_board(int size)
{
  //printf("Initializing Board.\n");
  if (size < MIN_BRD_SIZE)
    return NULL;

  board * brd = malloc(sizeof(board));
  if (!brd)
    return NULL;

  brd->size = size;
  brd->num_pits = PIT_NUM;
  brd->locs = malloc((size*size)*sizeof(int));
    if (!brd->locs)
    {
      free(brd);
      return NULL;
    }


  brd->pits = malloc(brd->num_pits * sizeof(int));
    if(!brd->pits)
    {
      free(brd->locs);
      free(brd);
      return NULL;
    }
  make_unknown_board(brd);
  init_objects_unplaced(brd);
  place_basilisk(brd);
  place_pits(brd);
  place_player(brd);

  return brd;
}

void init_objects_unplaced(board * brd)
{
  int unplaced = -1;
  brd->basilisk = unplaced;
  brd->player_loc = unplaced;
  for (int i = 0; i < brd->num_pits; i++)
    brd->pits[i] = unplaced;
}

void make_unknown_board(board * brd)
{
  for (int i = 0; i < (brd->size * brd->size); i++)
  {
    brd->locs[i] = UNKNOWN;
  }
}

void place_basilisk(board * brd)
{
  int loc = place_object(brd);
  brd->basilisk = loc;
}

void place_pits (board * brd)
{
  int loc;
  for (int i = 0; i < brd->num_pits; i++)
  {
    loc = place_object(brd);
    brd->pits[i] = loc;
  }
}

void place_player (board * brd)
{
  int loc = place_object(brd);
  brd->player_loc = loc;
}


int place_object(board * brd)
{
  int r, err = 1;
  while(err)
  {
    r  = get_random_loc(brd);
    err = check_occupied(brd, r);
  }
  return r;
}


int get_random_loc(board * brd)
{
  srand((unsigned int)time(NULL));
  int r = rand() % (brd->size * brd->size); // returns random number 0-(size^2 -1)
  return r;
}


int check_occupied(board * brd, int loc)
{
  // malformed input, return space is occupied
  if (!brd || !is_on_board(brd, loc))
    return -1;

  /* Check for pits, basilisk, and player_loc*/
  if((brd->basilisk == loc || brd->player_loc == loc))
    {
      return 1;
    }
  for (int i = 0; i < brd->num_pits; i++)
  {
    if(brd->pits[i] == loc) // try again for another spot
      {
        return 2;
      }
  }
  return 0;
}


void print_board (board * brd, arrows * arr)
{
  int loc;
  if(!brd)
    return;
  for (int i = 0; i < (brd->size * brd->size); i++)
  {
    if((i % brd->size) == 0)
      printf("\n");
    loc = brd->locs[i];

    /* Check for arrows */
    for (int i = 0; i < ARROW_START_NUM - arr->user_rem; i++)
    {
      if(arr->locs[i] == loc)
        {
          if(loc != UNKNOWN)
            loc = ARROW;
          break;
        }
    }

    printf("[ ");
    switch(loc)
    {
      case C_PIT:
        printf(" P");
        break;
      case C_BAS:
        printf(" B");
        break;
      case ARROW:
        printf(" A");
        break;
      case PLAYER:
        printf("ME");
        break;
      case SAFE:
        printf("  ");
        break;
      case PIT:
        printf("P?");
        break;
      case BAS:
        printf("B?");
        break;
      default: // UNKOWN
        printf("  ");
        break;
    }

    printf(" ]");
  }
  printf("\n");
  // printf("basilisk location: %d\n", brd->basilisk);
  // printf("pit locations: ");
  // for (int ii; ii < brd->num_pits; ii++)
  //   printf("%d ", brd->pits[ii]);
  // printf("\n");
  // printf("player location: %d\n", brd->player_loc);
}

void pick_up_arrow(board * brd, arrows * arr)
{
  int loc = brd->player_loc;
  int ii;
  for (int i = 0; i < (ARROW_START_NUM - arr->user_rem); i++)
  {
    if(arr->locs[i] == loc) // player has picked up the arrow
    {
      for (ii = i; ii < ARROW_START_NUM - arr->user_rem - 1; ii++)
      {
        arr->locs[ii] = arr->locs[ii+1];
      }
      if (ii != ARROW_START_NUM)
        arr->locs[ii] = -1;

      arr->user_rem++; // 5 arrows, shot 5, pick up num 1
      printf("You picked up an arrow!\n");
      break;
    }
  }
}


void place_arrow (board * brd, arrows * arr)
{
  printf("Placing arrow...\n");
  int i, loc, err;
  do
  {
    err = 0;
    loc = place_object(brd);

    // Modulatiry for arrow starting numbers greater than or equal to 2
    if (arr->user_rem < ARROW_START_NUM) // check if the other arrow occupies the location
      {
        for (i = 0; i < ARROW_START_NUM - (arr->user_rem); i++)
        {
          if (arr->locs[i] == loc)
            {
              err = 1;
              break;
            }
        }
      }
  } while(err);
  arr->locs[2 - (arr->user_rem)] = loc; // place the arrow
  //printf("Placing arrow at loc %d\n", loc);
}

void delete_board(board * brd)
{
  free(brd->pits);
  free(brd->locs);
  free(brd);
}

void delete_arrow(arrows * arr)
{
  free(arr->locs);
  free(arr);
}

death check_movement(board * brd, move m)
{
  int loc = brd->player_loc;
  int prev = loc;
  int err;
  switch (m)
  {
    case UP:
      loc -= brd->size;
      err = check_occupied(brd, loc);
      break;
    case DOWN:
      loc += brd->size;
      err = check_occupied(brd, loc);
      break;
    case LEFT:
      loc -= 1;
      err = check_occupied(brd, loc);
      break;
    case RIGHT:
      loc += 1;
      err = check_occupied(brd, loc);
      break;
    default:
      return OUT_OF_BOUNDS;
  }
    if(err == 2)
      return PIT_D;
    else if (err == 1)
      return BASILISK_D;
    else if (err < 0)
      return OUT_OF_BOUNDS;
    else if (!check_wrapping(brd, brd->player_loc, loc))
      return OUT_OF_BOUNDS;

  brd->locs[prev] = SAFE;
  brd->player_loc = loc;
  return ALIVE;
}

/* Checks for and prevents wrapping from one side of the board to the other
Returns 0 if wrapping is found, 1 for legal move */
int check_wrapping(board * brd, int prev_loc, int new_loc)
{

  /* See if either loc is on the side */
  if((prev_loc % brd->size) == 0 || (new_loc % 5) == 0)
  {
    int side, other;
    /* Find which is the side loc */
    if ((new_loc % brd->size) == 0)
    {
      side = new_loc;
      other = prev_loc;
    }
    else
    {
      side = prev_loc;
      other = new_loc;
    }

    if((side - other) == 1) // condition for wrapping
      return 0;
  }
  return 1;
}


void update_board(board * brd)
{
  int loc = brd->player_loc;
  brd->locs[loc] = PLAYER;      // mark ME on the board
  int above = loc - brd->size;
  int below = loc + brd->size;
  int left = loc - 1;
  int right = loc + 1;

  /* 1 Basilisk, 2 pits*/
  if ((check_occupied(brd, above) == 1 && check_wrapping(brd, loc, above))
  || (check_occupied(brd, below) == 1 && check_wrapping(brd, loc, below))
  || (check_occupied(brd, left) == 1 && check_wrapping(brd, loc, left))
  || (check_occupied(brd, right) == 1 && check_wrapping(brd, loc, right)))
  {
    /* If the player hasn't found the location to be safe, mark it as
    potentially dangerous */
    if(brd->locs[above] != SAFE && is_on_board(brd, above) && check_wrapping(brd,loc, above))
      brd->locs[above] = BAS;
    if(brd->locs[below] != SAFE && is_on_board(brd, below) && check_wrapping(brd,loc, below))
      brd->locs[below] = BAS;
    if(brd->locs[left] != SAFE && is_on_board(brd, left) && check_wrapping(brd,loc, left))
      brd->locs[left] = BAS;
    if(brd->locs[right] != SAFE && is_on_board(brd, right) && check_wrapping(brd,loc, right))
      brd->locs[right] = BAS;
    printf("I hear panting (Basilisk nearby)\n");
  }
  /* There is a pit nearby */
  else if ((check_occupied(brd, above) == 2 && check_wrapping(brd, loc, above))
  || (check_occupied(brd, below) == 2 && check_wrapping(brd, loc, below))
  || (check_occupied(brd, left) == 2 && check_wrapping(brd, loc, left))
  || (check_occupied(brd, right) == 2 && check_wrapping(brd, loc, right)))
  {
    /* If the player hasn't found the location to be safe, mark it as
    potentially dangerous */
    if(brd->locs[above] != SAFE && is_on_board(brd, above) && check_wrapping(brd,loc, above))
      brd->locs[above] = PIT;
    if(brd->locs[below] != SAFE && is_on_board(brd, below) && check_wrapping(brd,loc, below))
      brd->locs[below] = PIT;
    if(brd->locs[left] != SAFE && is_on_board(brd, left) && check_wrapping(brd,loc, left))
      brd->locs[left] = PIT;
    if(brd->locs[right] != SAFE && is_on_board(brd, right) && check_wrapping(brd,loc, right))
      brd->locs[right] = PIT;
    printf("I feel a breeze (Pit Nearby)\n");
  }
  else // all safe
  {
    if(is_on_board(brd, above) && check_wrapping(brd,loc, above))
      brd->locs[above] = SAFE;
    if(is_on_board(brd, below) && check_wrapping(brd,loc, below))
      brd->locs[below] = SAFE;
    if(is_on_board(brd, left) && check_wrapping(brd,loc, left))
      brd->locs[left] = SAFE;
    if(is_on_board(brd, right) && check_wrapping(brd,loc, right))
      brd->locs[right] = SAFE;
    printf("Silence (Safe all around)\n");
  }
}


int is_on_board(board * brd, int loc)
{
  if(loc >= (brd->size * brd->size) || loc < 0)
    return 0;
  else
    return 1;
}

void end_game_show_locs(board * brd, arrows * arr)
{
  int ii, loc;
  for (int i = 0; i < (brd->size * brd->size); i++) // loop through every location on map
  {
    loc = SAFE; // default SAFE

    if(i == brd->player_loc) // mark player loc
      loc = PLAYER;

    for(ii = 0; ii < PIT_NUM; ii++) // mark pit locations
    {
      if(i == brd->pits[ii])
      {
        loc = C_PIT;
        break;
      }
    }

    for(ii = 0; ii < (ARROW_START_NUM - arr->user_rem); ii++) // mark arrow locs
    {
      if (i == arr->locs[ii])
      {
        loc = ARROW;
        break;
      }
    }
    if (i == brd->basilisk) // mark basilisk loc
      loc = C_BAS;

    brd->locs[i] = loc;
  }
}
