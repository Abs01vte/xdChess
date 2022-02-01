## Command line Interface
`xdChess file1 file2`
This is how you call xdChess with two files, one with moves for white and one for black.
### Standard Input
The dash character `-` represents standard input.
For Example:
`xdChess file1 -`
White's moves come from file1, black's moves come from standard input.
If it is given one file, it will divide the spaces evenly, alternating the moves for white and black.
### CMD args
- -a alternating. It alternates between white and black moves and is the default behavior.
- -m middle. "Split down the middle," cut the moves horizontally, not vertically.
- -i infinite. Run until checkmate, stalemate or lack of input.
- -x=<num> maximum amount of moves. Default is infinite.
- -b=<num> maximum bytes to read before exiting. 
