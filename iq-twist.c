//******************************************************************************

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

typedef struct
{
  uint8_t x;
  uint8_t y;
  uint8_t peg;
  uint8_t corner;
} coord_t;

typedef struct
{
  uint8_t id;
  uint8_t width;
  uint8_t height;
  uint8_t size;

  coord_t coord[5];
} token_t;

#define TOKENS 56
#define POSITIONS 8
#define VOID 0xff
#define WIDTH 8
#define HEIGHT 4
#define EMPTY '.'

typedef struct
{
  uint8_t x;
  uint8_t y;
} pivot_t;

typedef struct
{
  uint8_t token;
  uint8_t left;
  uint8_t bottom;
} position_t;

typedef struct
{
  position_t pos[POSITIONS];
  uint8_t board[WIDTH][HEIGHT];
} permutation_t;

//******************************************************************************
// >>> token

static token_t token[TOKENS] =
{
  // red l

  // a
  // AaA
  { // id w h s
    0, 3, 2, 4,
    { // x y m c
      { 0, 0, 1, 1 },
      { 1, 0, 0, 0 },
      { 2, 0, 1, 1 },
      { 0, 1, 0, 1 },
      { 0, 0, 0, 0 },
    }
  },

  // Aa
  // a
  // A
  { // id w h s
    0, 2, 3, 4,
    { // x y m c
      { 0, 0, 1, 1 },
      { 0, 1, 0, 0 },
      { 0, 2, 1, 1 },
      { 1, 2, 0, 1 },
      { 0, 0, 0, 0 },
    }
  },

  // AaA
  //   a
  { // id w h s
    0, 3, 2, 4,
    { // x y m c
      { 2, 0, 0, 1 },
      { 0, 1, 1, 1 },
      { 1, 1, 0, 0 },
      { 2, 1, 1, 1 },
      { 0, 0, 0, 0 },
    }
  },

  //  A
  //  a
  // aA
  { // id w h s
    0, 2, 3, 4,
    { // x y m c
      { 0, 0, 0, 1 },
      { 1, 0, 1, 1 },
      { 1, 1, 0, 0 },
      { 1, 2, 1, 1 },
      { 0, 0, 0, 0 },
    }
  },

  //   a
  // AaA
  { // id w h s
    0, 3, 2, 4,
    { // x y m c
      { 0, 0, 1, 1 },
      { 1, 0, 0, 0 },
      { 2, 0, 1, 1 },
      { 2, 1, 0, 1 },
      { 0, 0, 0, 0 },
    }
  },

  // A
  // a
  // Aa
  { // id w h s
    0, 2, 3, 4,
    { // x y m c
      { 0, 0, 1, 1 },
      { 1, 0, 0, 1 },
      { 0, 1, 0, 0 },
      { 0, 2, 1, 1 },
      { 0, 0, 0, 0 },
    }
  },

  // AaA
  // a
  { // id w h s
    0, 3, 2, 4,
    { // x y m c
      { 0, 0, 0, 1 },
      { 0, 1, 1, 1 },
      { 1, 1, 0, 0 },
      { 2, 1, 1, 1 },
      { 0, 0, 0, 0 },
    }
  },

  // aA
  //  a
  //  A
  { // id w h s
    0, 2, 3, 4,
    { // x y m c
      { 1, 0, 1, 1 },
      { 1, 1, 0, 0 },
      { 0, 2, 0, 1 },
      { 1, 2, 1, 1 },
      { 0, 0, 0, 0 },
    }
  },

  // red s

  //  bb
  // bB
  { // id w h s
    1, 3, 2, 4,
    { // x y m c
      { 0, 0, 0, 1 },
      { 1, 0, 1, 1 },
      { 1, 1, 0, 1 },
      { 2, 1, 0, 1 },
      { 0, 0, 0, 0 },
    }
  },

  // b
  // Bb
  //  b
  { // id w h s
    1, 2, 3, 4,
    { // x y m c
      { 1, 0, 0, 1 },
      { 0, 1, 1, 1 },
      { 1, 1, 0, 1 },
      { 0, 2, 0, 1 },
      { 0, 0, 0, 0 },
    }
  },

  //  Bb
  // bb
  { // id w h s
    1, 3, 2, 4,
    { // x y m c
      { 0, 0, 0, 1 },
      { 1, 0, 0, 1 },
      { 1, 1, 1, 1 },
      { 2, 1, 0, 1 },
      { 0, 0, 0, 0 },
    }
  },

  // b
  // bB
  //  b
  { // id w h s
    1, 2, 3, 4,
    { // x y m c
      { 1, 0, 0, 1 },
      { 0, 1, 0, 1 },
      { 1, 1, 1, 1 },
      { 0, 2, 0, 1 },
      { 0, 0, 0, 0 },
    }
  },

  // bb
  //  Bb
  { // id w h s
    1, 3, 2, 4,
    { // x y m c
      { 1, 0, 1, 1 },
      { 2, 0, 0, 1 },
      { 0, 1, 0, 1 },
      { 1, 1, 0, 1 },
      { 0, 0, 0, 0 },
    }
  },

  //  b
  // Bb
  // b
  { // id w h s
    1, 2, 3, 4,
    { // x y m c
      { 0, 0, 0, 1 },
      { 0, 1, 1, 1 },
      { 1, 1, 0, 1 },
      { 1, 2, 0, 1 },
      { 0, 0, 0, 0 },
    }
  },

  // bB
  //  bb
  { // id w h s
    1, 3, 2, 4,
    { // x y m c
      { 1, 0, 0, 1 },
      { 2, 0, 0, 1 },
      { 0, 1, 0, 1 },
      { 1, 1, 1, 1 },
      { 0, 0, 0, 0 },
    }
  },

  //  b
  // bB
  // b
  { // id w h s
    1, 2, 3, 4,
    { // x y m c
      { 0, 0, 0, 1 },
      { 0, 1, 0, 1 },
      { 1, 1, 1, 1 },
      { 1, 2, 0, 1 },
      { 0, 0, 0, 0 },
    }
  },

  // yellow i

  // Ccc
  { // id w h s
    2, 3, 1, 3,
    { // x y m c
      { 0, 0, 1, 1 },
      { 1, 0, 0, 0 },
      { 2, 0, 0, 1 },
      { 0, 0, 0, 0 },
      { 0, 0, 0, 0 },
    }
  },

  // C
  // c
  // c
  { // id w h s
    2, 1, 3, 3,
    { // x y m c
      { 0, 0, 0, 1 },
      { 0, 1, 0, 0 },
      { 0, 2, 1, 1 },
      { 0, 0, 0, 0 },
      { 0, 0, 0, 0 },
    }
  },

  // ccC
  { // id w h s
    2, 3, 1, 3,
    { // x y m c
      { 0, 0, 0, 1 },
      { 1, 0, 0, 0 },
      { 2, 0, 1, 1 },
      { 0, 0, 0, 0 },
      { 0, 0, 0, 0 },
    }
  },

  // c
  // c
  // C
  { // id w h s
    2, 1, 3, 3,
    { // x y m c
      { 0, 0, 1, 1 },
      { 0, 1, 0, 0 },
      { 0, 2, 0, 1 },
      { 0, 0, 0, 0 },
      { 0, 0, 0, 0 },
    }
  },

  // yellow x

  //  d
  //  dD
  // DD
  { // id w h s
    3, 3, 3, 5,
    { // x y m c
      { 0, 0, 1, 1 },
      { 1, 0, 1, 1 },
      { 1, 1, 0, 0 },
      { 2, 1, 1, 1 },
      { 1, 2, 0, 1 },
    }
  },

  // D
  // Ddd
  //  D
  { // id w h s
    3, 3, 3, 5,
    { // x y m c
      { 1, 0, 1, 1 },
      { 0, 1, 1, 1 },
      { 1, 1, 0, 0 },
      { 2, 1, 0, 1 },
      { 0, 2, 1, 1 },
    }
  },

  //  DD
  // Dd
  //  d
  { // id w h s
    3, 3, 3, 5,
    { // x y m c
      { 1, 0, 0, 1 },
      { 0, 1, 1, 1 },
      { 1, 1, 0, 0 },
      { 1, 2, 1, 1 },
      { 2, 2, 1, 1 },
    }
  },

  //  D
  // ddD
  //   D
  { // id w h s
    3, 3, 3, 5,
    { // x y m c
      { 2, 0, 1, 1 },
      { 0, 1, 0, 1 },
      { 1, 1, 0, 0 },
      { 2, 1, 1, 1 },
      { 1, 2, 1, 1 },
    }
  },

  //  d
  // Dd
  //  DD
  { // id w h s
    3, 3, 3, 5,
    { // x y m c
      { 1, 0, 1, 1 },
      { 2, 0, 1, 1 },
      { 0, 1, 1, 1 },
      { 1, 1, 0, 0 },
      { 1, 2, 0, 1 },
    }
  },

  //  D
  // Ddd
  // D
  { // id w h s
    3, 3, 3, 5,
    { // x y m c
      { 0, 0, 1, 1 },
      { 0, 1, 1, 1 },
      { 1, 1, 0, 0 },
      { 2, 1, 0, 1 },
      { 1, 2, 1, 1 },
    }
  },

  // DD
  //  dD
  //  d
  { // id w h s
    3, 3, 3, 5,
    { // x y m c
      { 1, 0, 0, 1 },
      { 1, 1, 0, 0 },
      { 2, 1, 1, 1 },
      { 0, 2, 1, 1 },
      { 1, 2, 1, 1 },
    }
  },

  //   D
  // ddD
  //  D
  { // id w h s
    3, 3, 3, 5,
    { // x y m c
      { 1, 0, 1, 1 },
      { 0, 1, 0, 1 },
      { 1, 1, 0, 0 },
      { 2, 1, 1, 1 },
      { 2, 2, 1, 1 },
    }
  },
  

  // green l

  // e
  // EE
  { // id w h s
    4, 2, 2, 3,
    { // x y m c
      { 0, 0, 1, 1 },
      { 1, 0, 1, 1 },
      { 0, 1, 0, 1 },
      { 0, 0, 0, 0 },
      { 0, 0, 0, 0 },
    }
  },

  // Ee
  // E
  { // id w h s
    4, 2, 2, 3,
    { // x y m c
      { 0, 0, 1, 1 },
      { 0, 1, 1, 1 },
      { 1, 1, 0, 1 },
      { 0, 0, 0, 0 },
      { 0, 0, 0, 0 },
    }
  },

  // EE
  // e
  { // id w h s
    4, 2, 2, 3,
    { // x y m c
      { 0, 0, 0, 1 },
      { 0, 1, 1, 1 },
      { 1, 1, 1, 1 },
      { 0, 0, 0, 0 },
      { 0, 0, 0, 0 },
    }
  },

  //  E
  // eE
  { // id w h s
    4, 2, 2, 3,
    { // x y m c
      { 0, 0, 0, 1 },
      { 1, 0, 1, 1 },
      { 1, 1, 1, 1 },
      { 0, 0, 0, 0 },
      { 0, 0, 0, 0 },
    }
  },

  //  e
  // EE  
  { // id w h s
    4, 2, 2, 3,
    { // x y m c
      { 0, 0, 1, 1 },
      { 1, 0, 1, 1 },
      { 1, 1, 0, 1 },
      { 0, 0, 0, 0 },
      { 0, 0, 0, 0 },
    }
  },

  // E
  // Ee
  { // id w h s
    4, 2, 2, 3,
    { // x y m c
      { 0, 0, 1, 1 },
      { 0, 1, 1, 1 },
      { 1, 0, 0, 1 },
      { 0, 0, 0, 0 },
      { 0, 0, 0, 0 },
    }
  },

  // EE
  // e
  { // id w h s
    4, 2, 2, 3,
    { // x y m c
      { 0, 0, 0, 1 },
      { 0, 1, 1, 1 },
      { 1, 1, 1, 1 },
      { 0, 0, 0, 0 },
      { 0, 0, 0, 0 },
    }
  },

  // eE
  //  E
  { // id w h s
    4, 2, 2, 3,
    { // x y m c
      { 1, 0, 1, 1 },
      { 0, 1, 0, 1 },
      { 1, 1, 1, 1 },
      { 0, 0, 0, 0 },
      { 0, 0, 0, 0 },
    }
  },

  // green t

  //  F
  // Fff
  { // id w h s
    5, 3, 2, 4,
    { // x y m c
      { 0, 0, 1, 1 },
      { 1, 0, 0, 0 },
      { 2, 0, 0, 1 },
      { 1, 1, 1, 1 },
      { 0, 0, 0, 0 },
    }
  },

  // F
  // fF
  // f
  { // id w h s
    5, 3, 2, 4,
    { // x y m c
      { 0, 0, 0, 1 },
      { 0, 1, 0, 0 },
      { 1, 1, 1, 1 },
      { 0, 2, 1, 1 },
      { 0, 0, 0, 0 },
    }
  },

  // ffF
  //  F
  { // id w h s
    5, 3, 2, 4,
    { // x y m c
      { 1, 0, 1, 1 },
      { 0, 1, 0, 1 },
      { 1, 1, 0, 0 },
      { 2, 1, 1, 1 },
      { 0, 0, 0, 0 },
    }
  },

  //  f
  // Ff
  //  F
  { // id w h s
    5, 3, 2, 4,
    { // x y m c
      { 1, 0, 1, 1 },
      { 0, 1, 1, 1 },
      { 1, 1, 0, 0 },
      { 1, 2, 0, 1 },
      { 0, 0, 0, 0 },
    }
  },

  //  F
  // ffF
  { // id w h s
    5, 3, 2, 4,
    { // x y m c
      { 0, 0, 0, 1 },
      { 1, 0, 0, 0 },
      { 2, 0, 1, 1 },
      { 1, 1, 1, 1 },
      { 0, 0, 0, 0 },
    }
  },

  // f
  // fF
  // F
  { // id w h s
    5, 3, 2, 4,
    { // x y m c
      { 0, 0, 1, 1 },
      { 0, 1, 0, 0 },
      { 1, 1, 1, 1 },
      { 0, 2, 0, 1 },
      { 0, 0, 0, 0 },
    }
  },

  // Fff
  //  F
  { // id w h s
    5, 3, 2, 4,
    { // x y m c
      { 1, 0, 1, 1 },
      { 0, 1, 1, 1 },
      { 1, 1, 0, 0 },
      { 2, 1, 0, 1 },
      { 0, 0, 0, 0 },
    }
  },

  //  F
  // Ff
  //  f
  { // id w h s
    5, 3, 2, 4,
    { // x y m c
      { 1, 0, 0, 1 },
      { 0, 1, 1, 1 },
      { 1, 1, 0, 0 },
      { 1, 2, 1, 1 },
      { 0, 0, 0, 0 },
    }
  },

  // blue i

  // gGgg
  { // id w h s
    6, 4, 1, 4,
    { // x y m c
      { 0, 0, 0, 1 },
      { 1, 0, 1, 0 },
      { 2, 0, 0, 0 },
      { 3, 0, 0, 1 },
      { 0, 0, 0, 0 },
    }
  },

  // g
  // G
  // g
  // g  
  { // id w h s
    6, 1, 4, 4,
    { // x y m c
      { 0, 0, 0, 1 },
      { 0, 1, 0, 0 },
      { 0, 2, 1, 0 },
      { 0, 3, 0, 1 },
      { 0, 0, 0, 0 },
    }
  },

  // ggGg
  { // id w h s
    6, 4, 1, 4,
    { // x y m c
      { 0, 0, 0, 1 },
      { 1, 0, 0, 0 },
      { 2, 0, 1, 0 },
      { 3, 0, 0, 1 },
      { 0, 0, 0, 0 },
    }
  },

  // g
  // g
  // G
  // g
  { // id w h s
    6, 1, 4, 4,
    { // x y m c
      { 0, 0, 0, 1 },
      { 0, 1, 1, 0 },
      { 0, 2, 0, 0 },
      { 0, 3, 0, 1 },
      { 0, 0, 0, 0 },
    }
  },

  // blue b

  // hhh
  // HH
  { // id w h s
    7, 3, 2, 5,
    { // x y m c
      { 0, 0, 1, 1 },
      { 1, 0, 1, 1 },
      { 0, 1, 0, 1 },
      { 1, 1, 0, 0 },
      { 2, 1, 0, 1 },
    }
  },

  // Hh
  // Hh
  //  h
  { // id w h s
    7, 2, 3, 5,
    { // x y m c
      { 1, 0, 0, 1 },
      { 0, 1, 1, 1 },
      { 1, 1, 0, 0 },
      { 0, 2, 1, 1 },
      { 1, 2, 0, 1 },
    }
  },

  //  HH
  // hhh
  { // id w h s
    7, 3, 2, 5,
    { // x y m c
      { 0, 0, 0, 1 },
      { 1, 0, 0, 0 },
      { 2, 0, 0, 1 },
      { 1, 1, 1, 1 },
      { 2, 1, 1, 1 },
    }
  },

  // h
  // hH
  // hH
  { // id w h s
    7, 2, 3, 5,
    { // x y m c
      { 0, 0, 0, 1 },
      { 1, 0, 1, 1 },
      { 0, 1, 0, 0 },
      { 1, 1, 1, 1 },
      { 0, 2, 0, 1 },
    }
  },

  // hhh
  //  HH
  { // id w h s
    7, 3, 2, 5,
    { // x y m c
      { 1, 0, 1, 1 },
      { 2, 0, 1, 1 },
      { 0, 1, 0, 1 },
      { 1, 1, 0, 0 },
      { 2, 1, 0, 1 },
    }
  },

  //  h
  // Hh
  // Hh
  { // id w h s
    7, 2, 3, 5,
    { // x y m c
      { 0, 0, 1, 1 },
      { 1, 0, 0, 1 },
      { 0, 1, 1, 1 },
      { 1, 1, 0, 0 },
      { 1, 2, 0, 1 },
    }
  },

  // HH
  // hhh
  { // id w h s
    7, 3, 2, 5,
    { // x y m c
      { 0, 0, 0, 1 },
      { 1, 0, 0, 0 },
      { 2, 0, 0, 1 },
      { 0, 1, 1, 1 },
      { 1, 1, 1, 1 },
    }
  },

  // hH
  // hH
  // h
  { // id w h s
    7, 2, 3, 5,
    { // x y m c
      { 0, 0, 0, 1 },
      { 0, 1, 0, 0 },
      { 1, 1, 1, 1 },
      { 0, 2, 0, 1 },
      { 1, 2, 1, 1 },
    }
  },
};

// <<<
//******************************************************************************
// >>> pivot

static pivot_t pivot[WIDTH * HEIGHT] =
{
  { 0, 0 }, // corners
  { 7, 0 },
  { 7, 3 },
  { 0, 3 },

  { 1, 0 }, // edges
  { 2, 0 },
  { 3, 0 },
  { 4, 0 },
  { 5, 0 },
  { 6, 0 },
  { 7, 1 },
  { 7, 2 },
  { 6, 3 },
  { 5, 3 },
  { 4, 3 },
  { 3, 3 },
  { 2, 3 },
  { 1, 3 },
  { 0, 2 },
  { 0, 1 },
  
  { 1, 1 }, // center
  { 2, 1 },
  { 3, 1 },
  { 4, 1 },
  { 5, 1 },
  { 6, 1 },
  { 6, 2 },
  { 5, 2 },
  { 4, 2 },
  { 3, 2 },
  { 2, 2 },
  { 1, 2 },
};

// <<<
//******************************************************************************

void print(const permutation_t *perm)
{
  for (int y = HEIGHT - 1; y >= 0; y--)
  {
    for (int x = 0; x < WIDTH; x++)
      putchar(perm->board[x][y]);
    putchar('\n');
  }
  putchar('\n');
}

int solve(int tokens, int search, const permutation_t *perm)
{
  uint8_t x, y;
  for (; search < WIDTH * HEIGHT; search++)
  {
    x = pivot[search].x;
    y = pivot[search].y;
    if (perm->board[x][y] == EMPTY)
      break;
  }
  assert(search < WIDTH * HEIGHT);

  int n = 0;
  for (int t = 0; t < TOKENS; t++)
  {
    uint8_t id = token[t].id;
    if (perm->pos[id].token != VOID)
      continue;

    for (int corner = 0; corner < token[t].size; corner++)
    {
      if (!token[t].coord[corner].corner)
	continue;

      int xx = token[t].coord[corner].x;
      int yy = token[t].coord[corner].y;

      if (x - xx < 0
        || x - xx + token[t].width - 1 > WIDTH
	|| y - yy < 0
        || y - yy + token[t].height - 1 > HEIGHT)
        continue;

      for (int i = 0; i < token[t].size; i++)
      {
	int xxx = token[t].coord[i].x + x - xx;
	int yyy = token[t].coord[i].y + y - yy;
	if (perm->board[xxx][yyy] != EMPTY)
	  goto FAIL;
      }

      permutation_t check;
      memcpy(&check, perm, sizeof(check));

      check.pos[id].token = t;
      check.pos[id].left = x - xx;
      check.pos[id].bottom = y - yy;

      for (int i = 0; i < token[t].size; i++)
      {
	int xxx = token[t].coord[i].x + x - xx;
	int yyy = token[t].coord[i].y + y - yy;

	check.board[xxx][yyy] = token[t].coord[i].peg
	  ? 'A' + id
	  : 'a' + id;
      }

      if (tokens + 1 < POSITIONS)
	n += solve(tokens + 1, search + 1, &check);
      else
      {
	print(&check);
	n++;
      }
FAIL:
    }
  }
  
  return n;
}

//******************************************************************************

int main()
{
  permutation_t perm;
  memset(&perm.pos, 0, sizeof(perm.pos));
  memset(&perm.board, EMPTY, sizeof(perm.board));
  for (int i = 0; i < POSITIONS; i++)
    perm.pos[i].token = VOID;

  int n = solve(0, 0, &perm);
  fprintf(stderr, "perumtations=%d\n", n);

  return EXIT_SUCCESS;
}

//******************************************************************************
