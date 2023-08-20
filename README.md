# IQ-Twist

[IQ-Twist](https://www.smartgames.eu/de/spiele-f%C3%BCr-einen-spieler/iq-twist)
is a puzzle game played on an 8x4 grid.  Positions on the grid are flagged with
colored pegs, e.g.  following exercises in a booklet.  8 tiles, also colored,
have to be placed on the Grid, so that
- the grid is fully covered
- if tiles are placed above pegs, their colors must match
- tiles can only placed above pegs, where tiles have a hole


## Mathematical Analysis

Without pegs set, the tiles can be arranged in 388288 different permutations
(placement, rotations, reflections).


## Computational Results

All possible permutation can easily enumerated in 6.2s on an Intel Core i7-9700K.

The puzzles 19 to 120 from the booklet can be solved on average in 1.1ms.

### Build

    gcc -O3 -o generate iq-twist.c
    generate > solutions # enumerate all arrangements
    gcc -O3 -o solve solver.c
    solve solutions < puzzles
