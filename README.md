# What

Chess game randomizer/creater. It's so random XD!

## Command line Interface

xdChess takes between [0-2] files as input.

Example use:

`xdChess file1 file2`

The above line will call xdcess with two files as input: file1 will generate moves for white, and
file2 will generate moves for black.

Example input for two files:
File 1:
`e4
Nf3
Bc4`
File 2:
`e5
Nc6
Bc5`

`xdChess file1`

Call xdchess with one file as input. Both white and black's moves are derived
from this file.

You can also call xdChess with no input, in which case it read from stdin.

Example input for one file:
`e4 e5
Nf3 Nc6
Bc4 Bc5`

Example:

`cat myFile.txt | xdchess`

### Standard Input

The dash character `-` represents standard input.

For Example:

`xdChess file1 -`

White's moves come from file1, black's moves come from standard input.
If it is given one file, it will divide the spaces evenly, alternating the moves for white and black.

`xdChess - file2`

This is the same as before, except white's moves now come from stdin and
black's moves come from file2.

Calling xdchess with only stdin as an input file is the same as calling xdchess with no input files.

In other words: `cat test.txt | xdchess -` is the same as `cat test.txt | xdchess`

### CMD args

| Flag | Argument | What        | Explanation                                                       |
|:-----|:---------|:------------|:------------------------------------------------------------------|
| \-a  |          | Alternating | Alternates between white and black (default behaviour)            |
| \-m  |          | Middle      | Read entire file and split moves down the moddle                  |
| \-x  | integer  | Max moves   | Maximum amount of moves to create before quitting                 |
| \-b  | integer  | Max bytes   | Maximum amount of bytes to read from a file before deriving moves |

Flags with no argument are called on their own.

Example:

`xdchess -a`

Flags that take arguments use the `-f=arg` syntax.

Example:

`xdchess -x=420`
