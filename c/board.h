#ifndef _BOARD_H_
#define _BOARD_H_

typedef struct _arrows{

  int user_rem;
  int * locs; // missed shot arrow locations

} arrows;

typedef enum _shot {
  HIT = 0,
  MISS = 1,
  NO_ARROW = 2,
}shot;

/* Player Visualization of The Map*/
typedef enum _space {
  UNKNOWN = 0,
  SAFE = 1,
  PIT = 2,
  BAS = 3,
  PLAYER = 4,
  ARROW = 5,
  // Confirmed locations for end of game display
  C_PIT = 6,
  C_BAS = 7,
} space;

/* Direction of player move */
typedef enum _move {
  UP = 0,
  DOWN = 1,
  LEFT = 2,
  RIGHT = 3,
  INVALID = 4,
  HELP  = 5,
  SHOOT_UP = 6,
  SHOOT_DOWN = 7,
  SHOOT_LEFT = 8,
  SHOOT_RIGHT = 9,
} move;

typedef enum _death {
  ALIVE = 0, // SAFE
  OUT_OF_BOUNDS = 1, // OFF of the map
  PIT_D = 2,
  BASILISK_D = 3,
} death;


typedef struct _board{

  /* Locations of objects*/
  int basilisk;
  int * pits;
  int player_loc;

  int num_pits;
  int size;    // Lenght of Locations = size^2
  int * locs;  // player view of board

} board;


/* Initialize arrow object */
arrows * init_arrows(void);
/* Initialize board object */
board * init_board (int size);
/* Initializes objects as unplaced */
void init_objects_unplaced(board * brd);
/* Initialize player's view of board as all unknown */
void make_unknown_board(board * brd);
void place_basilisk (board * brd);
void place_pits (board * brd);
void place_player(board * brd);
/* Fnds an available location and returns it */
int place_object(board * brd);
/* Returns a random location given the dimensions of the board */
int get_random_loc(board * brd);
/* Checks if the location is occupied by an object on the board
Returns 0 if space is empty and 1 if space is occupied, -1 for bad input*/
int check_occupied(board * brd, int loc);
/* Prints the board to stdout */
void print_board(board * brd, arrows * arr);
void pick_up_arrow(board * brd, arrows * arr);
/* Places an arrow in a random unoccupied location */
void place_arrow (board * brd, arrows * arr);
void delete_board(board * brd);
void delete_arrow(arrows * arr);
/* Move player's location
Returns SAFE for valid move, type of death, or WALL invalid*/
death check_movement(board * brd, move m);
/* Checks for wrapping from one side of the board to the other
Returns 0 if wrapping is found, 1 for legal move */
int check_wrapping(board * brd, int prev_loc, int new_loc);
/* Updates the user's map to reflect current situation
and print to stdin user updates */
void update_board(board * brd);
/* Verifies a locaation is within the board's boundaries
Returns 1 if loc is on the board */
int is_on_board(board * brd, int loc);
/* Separates each new board */
void display_new_move(void);
void end_game_show_locs(board * brd, arrows * arr);
#endif
