/* Author:      Vincent Sevilla
 * Filename:    HexPieces.h
 * Description: Header file for the HexPieces class. Contains
 *                the template code to solve the hex puzzle.
 */


#ifndef _HEXPIECES
#define _HEXPIECES

const int ROWSIZE = 7;
const int COLSIZE = 6;
const int ONE_SECOND = 1000000;
const int NUM_OF_ARGS = 2;
const std::string USAGE_PROMPT = "To run, please type in: ./hexexe 1\n" \
    "to indicate a display frame time of 1 second.\n" \
    "Feel free to change this value anywhere between " \
    "0 - 5 seconds.\n";
const std:: string USAGE_ERR = "Please only real numbers for your input!\n\n";

class HexPieces {
  public:
    /*time in seconds of how long to display the board for one frame*/
    int frameTime;
    
    HexPieces();
    bool isTheRandomSetSolvable(int randomSet[][COLSIZE], int displayFlag);
    void generateARandomSet(int puzzlePieces[][COLSIZE]);
    void displayASet(int puzzlePieces[][COLSIZE]);

  private:
    /*Represents which tiles are on the board:
      element 0 = piece currently on center tile,
      element 1 = piece currently on north tile,
      element 2 = piece currently on northeast tile, etc...*/
    int tilesOnTheBoard[ROWSIZE];

    void solveIt(int puzzlePieces[][COLSIZE], int displayFlag, int & solved, 
        int currentState);

    bool checkForDuplicates(int puzzlePieces[][COLSIZE], int currentState); 
    
    bool youCanShortCircuit(int currentState, int puzzlePieces[][COLSIZE]);

    bool aPieceDoesntFit(int currentState, int puzzlePieces[][COLSIZE]);

    std::string translateTheTilesToString(int puzzlePieces[][COLSIZE],
        int currentState);

    void displayThePicture(int puzzlePieces[][COLSIZE], int currentState);

    void writeALine(std::string lineFromTemplateFile, std::string encodedTiles);

    bool arePiecesTheSame(int puzzlePieces[][COLSIZE], int tileNumber);

    void rotateTile(int puzzlePieces[][COLSIZE], int tileNumber);
};


#endif
