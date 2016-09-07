/* Author:      Vincent Sevilla
 * Filename:    HexPieces.cpp
 * Description: Implementation file for the HexPieces class. Contains
 *                the code to solve the hex puzzle
 */

#include <iostream>
#include "HexPieces.h"
#include <string>
#include <fstream>
#include <unistd.h>
#include <cstdlib>
#include <ctime>

using namespace std;


/* Constructor:     HexPieces
 * Description:     Simply initializes the tiles on the board to be empty. 
 */
HexPieces::HexPieces() {
  for (int tileNumber = 0; tileNumber < ROWSIZE; tileNumber++) {
    tilesOnTheBoard[tileNumber] = -1;
  }
}



/*
 * Name:        isTheRandomSetSolvable
 * Prototype:   isTheRandomSetSolvable(int randomSet[][COLSIZE],
 *                  int displayFlag);
 * Description: This function checks to see if a tile will have 
 * Parameters:
 *    randomSet   - The current set of puzzle pieces
 *    displayFlag - Whether or not the user wants the puzzle to be
 *                    solved visually
 * Return:      true if the given set was solvable, false if not.
 */
bool HexPieces::isTheRandomSetSolvable(int randomSet[][COLSIZE], 
    int displayFlag) {
 
  int solved = 0; int & temp = solved;

  solveIt(randomSet, displayFlag, temp, 0);  

  return (bool)solved;
}



/*
 * Name:        solveIt 
 * Prototype:   solveIt(int puzzlePieces[][COLSIZE], int displayFlag, 
 *                  int & solved, int currentState); 
 * Description: This function attempts to solve the hex puzzle.  It tries to
 *              do so recursively by exhaustively cycling through all 7 possible
 *              puzzle pieces, placing a tile into a position if the tile is not
 *              on the board and if it fits. Since this function implements
 *              recursion, the base case is if either the reference parameter 
 *              solved becomes true or if all 7 tiles have been placed in the
 *              center position, in which case the given set of puzzle pieces
 *              is not solvable.
 * Parameters:
 *    puzzlePieces      - The current set of puzzle pieces
 *    displayFlag       - Flag indicating if the game board/rotations should
 *                         be displayed to stdout.
 *    solved            - A reference flag indicating if the puzzle has been
 *                        solved. It is one of two base cases.
 *    currentState      - Determines how many tiles have been placed on the
 *                          board depending on the current level of recursion.
 */
void HexPieces::solveIt(int puzzlePieces[][COLSIZE], int displayFlag, 
    int & solved, int currentState) {

  int currentPiece = -1, rotateCounter = 0, 
      pieceIsOnTheBoard = 0, tileNumber = 0;
  bool shortCircuit = false;

  /*keep trying to solve until the puzzle has been solved or you've 
    already tried placing all the remaining tiles*/
  while (!solved && ++currentPiece != ROWSIZE) {
      shortCircuit = false;
    
    /*check to see if this tile is already on the board*/
    while(tileNumber != ROWSIZE) {
      if (tilesOnTheBoard[tileNumber++] == currentPiece) {
        pieceIsOnTheBoard = 1;
        break;
      }
    }

    /*if its not on the board then try to place it*/
    if(!pieceIsOnTheBoard) {

      tilesOnTheBoard[currentState] = currentPiece;

      /*If its not the center tile, and you havent rotated 6 times,
        and the border numbers do not match, then keep rotating-trying
        to see if the puzzle piece will fit*/
      while(currentState && rotateCounter != COLSIZE && 
          aPieceDoesntFit(currentState, puzzlePieces)) {
   
        /*see if you can short circuit, that is, stop rotating because you
          know absolutely this tile wont work, if one adjacent edges match 
          but the other doesnt*/
        if ((shortCircuit = youCanShortCircuit(currentState, puzzlePieces))) {
          break;
        }
          
        rotateCounter++;
        rotateTile(puzzlePieces, currentPiece);
        
        if (displayFlag) {
          displayThePicture(puzzlePieces, currentState + 1);
        }
      }

      if (shortCircuit) {
        /*necessary resets*/
        tilesOnTheBoard[currentState] = -1; rotateCounter = 0;
        pieceIsOnTheBoard = 0; tileNumber = 0;
        continue;
      }
        

      /*make sure it displays in case we didnt need to rotate the tile*/
      if (displayFlag && !rotateCounter) { 
        displayThePicture(puzzlePieces, currentState + 1);
      }

      /*if we didnt rotate too many times then its important border
        numbers must have matched those edges they were touching*/
      if (rotateCounter != COLSIZE) {
    
        /*this is an intelligent check to see if the surrounding tile
          border numbers are repeats. If they are, then we know we should
          move onto trying to place another piece.  For instance, no tile will
          work if the 2-3 edges its touching have the same number (because
          no tile can have 2 of the same number for its border numbers)*/
        if(!checkForDuplicates(puzzlePieces, currentState + 1)) {
          /*necessary resets*/
          tilesOnTheBoard[currentState] = -1; rotateCounter = 0;
          pieceIsOnTheBoard = 0; tileNumber = 0;
          continue;
        }
       
        /*see if we've filled all the slots on the board*/
        if (currentState == COLSIZE) { 
          /*if this point is reached then the puzzle has been solved*/
          solved = 1; tilesOnTheBoard[currentState] = -1;return;
        }

        /*recursively try to place a tile into the next position*/
        solveIt(puzzlePieces, displayFlag, solved, currentState + 1);
      }
    }
    
    /*necessary resets*/
    tileNumber = 0; pieceIsOnTheBoard = 0; rotateCounter = 0;
    tilesOnTheBoard[currentState] = -1;
  }
}



/*
 * Name:        checkForDuplicates 
 * Prototype:   checkForDuplicates(int puzzlePieces[][COLSIZE], 
 *                  int currentState);
 * Description: This function checks to see if a tile will have 
 *              surrounding tile edges that have the same number (in which
 *              case, the last tile that was placed on the board
 *              must be replaced)
 * Parameters:
 *    puzzlePieces      - The current set of puzzle pieces
 *    currentState      - Determines how many tiles have been placed on the
 *                          board depending on the current level of recursion.
 * Return:      true if there are no repeates, false if there are
 */
bool HexPieces::checkForDuplicates(int puzzlePieces[][COLSIZE], int
    currentState) {
  
  /*convenient variables to use to represent each puzzle piece*/
  int centerTile = tilesOnTheBoard[0], northTile = tilesOnTheBoard[1],
      northETile = tilesOnTheBoard[2], southETile = tilesOnTheBoard[3], 
      southTile = tilesOnTheBoard[4], southWTile = tilesOnTheBoard[5], 
      northWTile = tilesOnTheBoard[6];
  
  /*depending on the state, determine if there are repeated border numbers*/
  if(currentState == 2) {
    /*center/north*/
    return ((puzzlePieces[northTile][2] == puzzlePieces[centerTile][1]) ||
        (puzzlePieces[northTile][4] == puzzlePieces[centerTile][5])) 
        ? false : true;
  }
  else if (currentState == 3) {
    /*center/north/northeast*/
    return (puzzlePieces[centerTile][2] == puzzlePieces[northETile][3]) 
        ? false : true;
  }
  else if (currentState == 4) {
    /*center/north/northeast/southeast*/
    return (puzzlePieces[southETile][4] == puzzlePieces[centerTile][3]) 
        ? false : true;
  } 
  else if (currentState == 5) {  
    /*center/north/northeast/southeast/south*/
    return (puzzlePieces[southTile][5] == puzzlePieces[centerTile][4]) 
        ? false : true;
  } 
  else if (currentState == 6) {
    /*center/north/northeast/southeast/south/southwest*/
    return ((puzzlePieces[southWTile][0] == puzzlePieces[centerTile][5]) ||
        (puzzlePieces[northTile][4] == puzzlePieces[centerTile][5])||
        (puzzlePieces[northTile][4] == puzzlePieces[southWTile][0])) 
        ? false : true;     
  }


  return true;

}



/*
 * Name:        youCanShortCircuit
 * Prototype:   youCanShortCircuit(int currentState, 
 *                  int puzzlePieces[][COLSIZE]); 
 * Description: This function checks to see if you can "short circuit"
 *              a tile, ie stop it from being rotated as you know it
 *              cannot work in that position.For example if 1 of 2 or 3
 *              edges a tile is touching matches but another it is touching
 *              doesn't match, then you know it will not work.
 * Parameters:
 *    puzzlePieces      - The current set of puzzle pieces
 *    currentState      - Determines how many tiles have been placed on the
 *                          board depending on the current level of recursion.
 * Return:      true if you can stop rotating prematurely, false if you cant.
 */
bool HexPieces::youCanShortCircuit(int currentState, 
    int puzzlePieces[][COLSIZE]) {
    
  /*convenient variables to use to represent each puzzle piece*/
  int centerTile = tilesOnTheBoard[0], northTile = tilesOnTheBoard[1],
      northETile = tilesOnTheBoard[2], southETile = tilesOnTheBoard[3],
      southTile = tilesOnTheBoard[4], southWTile = tilesOnTheBoard[5],
      northWTile = tilesOnTheBoard[6];

  /*based on how many pieces on the board will determine which checks to do*/
  if (currentState == 2) {
    /*center/north/northeast*/
    if (((puzzlePieces[centerTile][1] == puzzlePieces[northETile][4]) &&
        (puzzlePieces[northTile][2] != puzzlePieces[northETile][5])) ||
        
        ((puzzlePieces[northTile][2] == puzzlePieces[northETile][5]) &&
         (puzzlePieces[centerTile][1] != puzzlePieces[northETile][4]))) {
            
      return true;
    }
  }
  else if (currentState == 3) {
    /*center/north/northeast/southeast*/
    if (((puzzlePieces[centerTile][2] == puzzlePieces[southETile][5]) &&
         (puzzlePieces[northETile][3] != puzzlePieces[southETile][0])) ||
        
        ((puzzlePieces[northETile][3] == puzzlePieces[southETile][0]) &&
         (puzzlePieces[centerTile][2] != puzzlePieces[northETile][5]))) {
            
      return true;
    } 
  }
  else if (currentState == 4) {
      /*center/north/northeast/southeast/south*/
    if (((puzzlePieces[centerTile][3] == puzzlePieces[southTile][0]) &&
         (puzzlePieces[southTile][1] != puzzlePieces[southETile][4])) ||
        
        ((puzzlePieces[southTile][1] == puzzlePieces[southETile][4]) &&
         (puzzlePieces[centerTile][3] != puzzlePieces[southTile][0]))) {
            
      return true;
    }
  }
  else if (currentState == 5) {
    /*center/north/northeast/southeast/south/southwest*/
    if (((puzzlePieces[centerTile][4] == puzzlePieces[southWTile][1]) &&
         (puzzlePieces[southTile][5] != puzzlePieces[southWTile][2])) ||
        
        ((puzzlePieces[southTile][5] == puzzlePieces[southWTile][2]) &&
         (puzzlePieces[centerTile][4] != puzzlePieces[southWTile][1]))) {
            
      return true;
    } 
  }
  else if (currentState == 6) {
    /*center/north/northeast/southeast/south/southwest/northwest*/
    if (((puzzlePieces[centerTile][5] == puzzlePieces[northWTile][2]) &&
         (puzzlePieces[southWTile][0] != puzzlePieces[northWTile][3])) ||
        
        ((puzzlePieces[southWTile][0] == puzzlePieces[northWTile][3]) &&
         (puzzlePieces[centerTile][5] != puzzlePieces[northWTile][2]))) {
            
      return true;
    }    
  }
  
  return false;
}



/*
 * Name:        aPieceDoesntFit 
 * Prototype:   aPieceDoesntFit(int currentState, int puzzlePieces[][COLSIZE]); 
 * Description: This function checks if a tile fits into a particular position
 *              on the board.
 * Parameters:
 *    currentState      - Indicates how many pieces are currently on the board
 *    puzzlePieces      - The current set of puzzle pieces
 * 
 * Return:      true if a piece doesnt fit on the board. false if it does fit
 */
bool HexPieces::aPieceDoesntFit(int currentState, int puzzlePieces[][COLSIZE]) {
    
  /*create easy to use variables for the puzzle pieces on the board right
    now*/
  int centerTile = tilesOnTheBoard[0], northTile = tilesOnTheBoard[1],
      northETile = tilesOnTheBoard[2], southETile = tilesOnTheBoard[3], 
      southTile = tilesOnTheBoard[4], southWTile = tilesOnTheBoard[5], 
      northWTile = tilesOnTheBoard[6];
    
  /*check to see if the touching edges of tiles have matching border numbers*/
  /*Of course, border numbers will vary depending on the position, which is
    why the currentState is necessary*/
  if(currentState == 1) {
    /*center/north*/
    return (puzzlePieces[northTile][3] == puzzlePieces[centerTile][0]) 
        ? false : true;
  }
  else if (currentState == 2) {
    /*center/north/northeast*/
    return ((puzzlePieces[centerTile][1] == puzzlePieces[northETile][4]) &&
        (puzzlePieces[northTile][2] == puzzlePieces[northETile][5])) 
        ? false : true;
  }
  else if (currentState == 3) {
    /*center/north/northeast/southeast*/
    return ((puzzlePieces[southETile][5] == puzzlePieces[centerTile][2]) &&
        (puzzlePieces[northETile][3] == puzzlePieces[southETile][0])) 
        ? false : true;
  } 
  else if (currentState == 4) {
    /*center/north/northeast/southeast/south*/
    return ((puzzlePieces[southTile][0] == puzzlePieces[centerTile][3]) &&
        (puzzlePieces[southTile][1] == puzzlePieces[southETile][4])) 
        ? false : true;
  } 
  else if (currentState == 5) {
    /*center/north/northeast/southeast/south/southwest*/
    return ((puzzlePieces[southWTile][1] == puzzlePieces[centerTile][4]) &&
        (puzzlePieces[southWTile][2] == puzzlePieces[southTile][5])) 
        ? false : true;
  } 
  else if (currentState == 6) {
    /*center/north/northeast/southeast/south/southwest/northwest*/
    return ((puzzlePieces[northWTile][2] == puzzlePieces[centerTile][5]) &&
        (puzzlePieces[southWTile][0] == puzzlePieces[northWTile][3]) &&
        (puzzlePieces[northTile][4] == puzzlePieces[northWTile][1])) 
        ? false : true;     
  } 

  return false;
}



/*
 * Name:        generateARandomSet 
 * Prototype:   generateARandomSet(int puzzlePieces[][COLSIZE]); 
 * Description: This function generates a random set of puzzle pieces,
 *                whilst making sure every tile has a unique sequence.
 * Parameters:
 *    puzzlePieces      - The current set of puzzle pieces
 */
void HexPieces::generateARandomSet(int puzzlePieces[][COLSIZE]) {
	srand(time(0));
	int upperBound = COLSIZE;
	int lowerBound = 1;
	int randomNum;
	bool thisNumWorks = false;
	bool indexCounter = 0;

  /*for all 7 possible tiles*/
	for (int row = 0; row < ROWSIZE; row++) {

    /*come up with all 6 border numbers*/
		for (int column = 0; column < COLSIZE; column++) {
			thisNumWorks = false;
      
      /*if its the 0th row and 0th column, just pick any number*/
			if (!row && !column) {
				puzzlePieces[row][column] = rand() % upperBound + 1;
			}
      /*otherwise if its the first column, also pick any number*/
			else if (!column) { 
				puzzlePieces[row][column] = rand() % upperBound + 1;
      }
      /*but if its an arbitrary tile/bordernumber*/
			else {
				randomNum = rand() % upperBound + 1;
        /*check if this randomNum has already been assigned to a previous
          bordernumber for this tile*/
				for (int z = column; z > 0; z--) {
          /*if its equal to any previous column, then this random num wont
            work*/
				  if (randomNum == puzzlePieces[row][z - 1]) {
					  thisNumWorks = false;
					  break;
					}
				  else {
					  thisNumWorks = true;
          }
				}

        /*if the number was unique then assign it to this tile*/
			  if (thisNumWorks) {
				  puzzlePieces[row][column] = randomNum;
        }
        /*otherwise try again for this bordernumber*/
			  else {
				  column--;
        }
			}
		}
	
    /*lastly as long as you have 2 puzzle pieces, check if this newly generated
      tile is unique compared to all those we've generated b4 this one*/
    if (row > 0 && arePiecesTheSame(puzzlePieces, row)) {
			row--;
	  }
  }

}



/*
 * Name:        displayASet
 * Prototype:   displayASet(int puzzlePieces[][COLSIZE]);
 * Description: This function writes out a set of puzzle pieces to stdout.
 * Parameters:
 *    puzzlePieces      - The current set of puzzle pieces
 */
void HexPieces::displayASet(int puzzlePieces[][COLSIZE]) {
	for (int tileNumber = 0; tileNumber < ROWSIZE; tileNumber++)
	{
		for (int borderNumber = 0; borderNumber < COLSIZE; borderNumber++)
		{
			cout << puzzlePieces[tileNumber][borderNumber] << " ";
		}
		cout << endl;
	}
}



/*
 * Name:        translateTheTilesToString
 * Prototype:   translateTheTilesToString(int puzzlePieces[][COLSIZE],
 *                  int currentState) 
 * Description: This function converts the puzzlepieces to one string. Every
 *                7th character is a tile number.  After a tile number, there
 *                are 6 characters representing the border number sequence for
 *                that tile.
 * Parameters:
 *    puzzlePieces      - The current set of puzzle pieces
 *    currentState  - indicates how many tiles are currently on the board
 * Return:      The resultant string 
 */
string HexPieces::translateTheTilesToString(int puzzlePieces[][COLSIZE], 
    int currentState) { 
	
  string formedString = "";
  
  /*for all the pieces currently on the board*/
    for (int state = 0; state < currentState; state++) {
    
    /*write the tile number first*/
    formedString = formedString += to_string(tilesOnTheBoard[state]);

    /*then write out the 6 border numbers for that particular tile*/
    for (int borderNumber = 0; borderNumber < COLSIZE; borderNumber++) {
      formedString = formedString += 
          to_string(puzzlePieces[tilesOnTheBoard[state]][borderNumber]);
    }
  }

	return formedString;
}



/*
 * Name:        displayThePicture
 * Prototype:   displayThePicture(int puzzlePieces[][COLSIZE],int currentState);
 * Description: This function is designed to display the current state of
 *              the puzzle board to the console depending on how many
 *              pieces are on the board.
 * Parameters:
 *    puzzlePieces  - the current set of puzzle pieces.
 *
 *    currentState  - indicates how many tiles are currently on the board
 */
void HexPieces::displayThePicture(int puzzlePieces[][COLSIZE],int currentState){
	
  string tempString = "", encodedTiles = "";
  int stringLength = 0;

  ifstream templateFile;


  /*make a string representing all the tiles*/
  encodedTiles = translateTheTilesToString(puzzlePieces,
      currentState); 

  /*open the correct file given the currentState of the board*/
	if (currentState == 1) {
    templateFile.open("template_0.txt", ios::in);
	}
	else if (currentState == 2) {
    templateFile.open("template_1.txt", ios::in);
  }
	else if (currentState == 3) {
    templateFile.open("template_2.txt", ios::in);
  }
	else if (currentState == 4) {
    templateFile.open("template_3.txt", ios::in);
  }
	else if (currentState == 5) {
    templateFile.open("template_4.txt", ios::in);
  }
	else if (currentState == 6) {
    templateFile.open("template_5.txt", ios::in);
  }
	else if (currentState == 7) {
    templateFile.open("template_6.txt", ios::in);
  }
     
  /*clear the console so you can display the rotations frame by frame*/
  system("clear");

  /*read a line from the template file and write it out w/ the correct
    numbers in place of the placeholders to stdout. Keep doing this until
    you've finished scanning the whole file*/
  while (!templateFile.eof()) {
    getline(templateFile, tempString);
    writeALine(tempString, encodedTiles);
  }

  /*close file*/
  templateFile.close();
  
  /*pause the frame for the desired amount of time*/
  usleep(frameTime);
}



/*
 * Name:        writeALine
 * Prototype:   writeALine(string lineFromTemplateFile, string encodedTiles); 
 * Description: It replaces placeholders in a given string
 *                  w/ the correct tile numbers/bordernumbers and
 *                  then prints this updated string to stdout.
 * Parameters:
 *    lineFromTemplateFile  - This is a line from a specific
 *        hex puzzle template file.  It has placeholders that 
 *        will be replaced w/ the correct border number from the
 *        correct Tile. The placeholders basically indicate
 *        which bordernumber from which tile to place in its
 *        position. (See a template file for reference)
 *
 *    encodedTiles          - A string that contains all the
 *        bordernumbers from every tile in a specific order.  Every
 *        7th number in this string is the tile number (0-6).
 */
void HexPieces::writeALine(string lineFromTemplateFile,
    string encodedTiles) {
	
  char tempChar;

  /*check every character in the line form the templateFile*/
	for (int i = 0; i < lineFromTemplateFile.length(); i++)
	{
    /*grab the next character*/
		tempChar = lineFromTemplateFile[i];

    /*if its a structural character from the hex diagram ignore it*/
		if (tempChar != ' ' || tempChar != '/' || tempChar != '_') {
      /*but if its a placeholder(ie an uppercase or lowercase letter, then
        replace the placeholder w/ the correct number from the encodedTiles 
        string*/
      if (tempChar >= 'a' && tempChar <= 'z') {
     		lineFromTemplateFile[i] = encodedTiles[tempChar - 'a'];
      }
      else if (tempChar >= 'A' && tempChar <= 'Z') {
    		lineFromTemplateFile[i] = encodedTiles[tempChar - 'A' + 26];
      }    
    }
	}

	/* write the updated line from the template file to stdout*/
    cout << lineFromTemplateFile << endl;
}



/*
 * Name:        arePiecesTheSame
 * Prototype:   arePiecesTheSame(int puzzlePieces[][COLSIZE], int tileNumber);
 * Description: This function checks if the given puzzle piece has a unique
 *              border number sequence compared to those before it.
 * Parameters:
 *    puzzlePieces      - The current set of puzzle pieces
 *    tileNumber        - The desired puzzle piece to compare to all others
 * Return:      true if the given puzzle piece has the same sequence as any
 *              other tile. false if its unique.
 */
bool HexPieces::arePiecesTheSame(int puzzlePieces[][COLSIZE], int tileNumber) {

  bool sameSequence = true;

    /*compare the given puzzle piece to all those before it*/
		for (int tempTile = tileNumber; tempTile > 0; tempTile--)
		{
      /*rotate the given puzzle piece COLSIZE many times*/
			for (int cnt = 0; cnt < COLSIZE; cnt++)
			{
				sameSequence = true;
        rotateTile(puzzlePieces, tileNumber);

        /*check if the two pieces have the same sequence*/
        for (int col = 0; col < COLSIZE; col++) {
          if (puzzlePieces[tileNumber][col] != puzzlePieces[tempTile - 1][col]){
            sameSequence = false;
          }
        }
       
        /*if the two pieces have the same sequence return true*/
        if (sameSequence) {
          return true;
        }
			}
		}
    return false;
		
}



/*
 * Name:        rotateTile
 * Prototype:   rotateTile(int puzzlePieces[][COLSIZE], int tileNumber); 
 * Description: This function rotates the requested tile once.
 * Parameters:  
 *    puzzlePieces      - The current set of puzzle pieces
 *    tileNumber        - The desired puzzle piece to rotate
 */
void HexPieces::rotateTile(int puzzlePieces[][COLSIZE], int tileNumber) {

  /*perform a shift by swapping the first w/ the last*/
	int temp = puzzlePieces[tileNumber][0];
	for (int borderNumber = 0; borderNumber < COLSIZE - 1; borderNumber++)
	{
    /*shift the sequence left*/
		puzzlePieces[tileNumber][borderNumber] 
        = puzzlePieces[tileNumber][borderNumber + 1];
	}

	puzzlePieces[tileNumber][COLSIZE - 1] = temp;
}

