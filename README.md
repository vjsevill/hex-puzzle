#Hex Puzzle Generator/Solver
The hex puzzle consists of 7 puzzle pieces. A puzzle piece is a hexagon with
numbers 1-6 inclusive along its edges.  No two puzzle pieces can have the same
sequence of border numbers and no puzzle piece can have a number repeat.

This program will continue to generate random sets of puzzle pieces until it
finds one that satisfies the above conditions.  When it does, it will attempt to
solve it.  If it cannot be solved, it repeats the above process until it does.
Then, using file i/o, this program will display all of the moves required to 
solve the puzzle at a specified frame-time.

###Compiling
To compile the program, type in at the command line 
`g++ -o hexexe Source.cpp HexPieces.cpp` 

###Running
To run the program, after compiling, type in at the command line 
`./hexexe frame-time`
where frame-time is a real number between 0 and 5 inclusive.  This represents 
the time per frame at which a move will be displayed.

The final frame of the display looks something like the following:
![screen shot 2016-09-07 at 5 55 26 pm](https://cloud.githubusercontent.com/assets/18255295/18333391/64782b86-7523-11e6-8c69-8bdd81b2e208.png)
