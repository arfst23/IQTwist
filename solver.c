//******************************************************************************

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <assert.h>

#define WIDTH 8
#define HEIGHT 4
#define LENGTH (WIDTH * HEIGHT + HEIGHT + 1)
#define SOLUTIONS 388288
#define BUF 40

typedef struct
{
  uint8_t offset;
  char color;
} constr_t;

//******************************************************************************

int main(int ac, char *av[])
{
  assert(ac == 2);
  const char *file = av[1];

  int fd = open(file, O_RDONLY);
  assert(fd >= 0);
  
  struct stat st;
  int ret = fstat(fd, &st);
  assert(!ret);
  size_t size = (size_t)st.st_size;
  assert(size);
  assert(st.st_size == SOLUTIONS * LENGTH);

  char *base = (char*)mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
  assert(base != MAP_FAILED);
  close(fd);

  char buf[BUF];

  for (;;)
  {
    char *s = fgets(buf, BUF, stdin);
    if (!s)
      break;
    fputs(s, stdout);

    constr_t constr[8];
    int constrs = 0;
    for (;;)
    {
      char *s = fgets(buf, BUF, stdin);
      if (!s || *s == '\n')
	break;

      assert(s[0] == 'r' || s[0] == 'y' || s[0] == 'g' || s[0] == 'b');
      assert(s[1] == ' ');
      assert(s[2] >= '1' && s[2] <= '8');
      assert(s[3] == ' ');
      assert(s[4] >= '1' && s[4] <= '4');
      assert(s[5] == '\n');

      int x = s[2] - '1';
      int y = s[4] - '1';
      int row = HEIGHT - 1 - y;
      int offset = row * (WIDTH + 1) + x;
      
      constr[constrs].offset = offset;
      constr[constrs].color = s[0];
      constrs++;
    }

    for (int solution = 0; solution < SOLUTIONS; solution++)
    {
      const char *s = base + solution * LENGTH;
      int c;
      for (c = 0; c < constrs; c++)
      {
	if (constr[c].color == 'r')
	{
	  if (s[constr[c].offset] != 'A' && s[constr[c].offset] != 'B')
	    break;
	}
	else if (constr[c].color == 'y')
	{
	  if (s[constr[c].offset] != 'C' && s[constr[c].offset] != 'D')
	    break;
	}
        else if (constr[c].color == 'g')
	{
	  if (s[constr[c].offset] != 'E' && s[constr[c].offset] != 'F')
	    break;
	}
        else if (constr[c].color == 'b')
	{
	  if (s[constr[c].offset] != 'G' && s[constr[c].offset] != 'H')
	    break;
	}
        else
	  assert(0);
      }

      if (c == constrs)
	fwrite(s, LENGTH, 1, stdout);
    }
  }

  munmap(base, size);

  return EXIT_SUCCESS;
}

//******************************************************************************
