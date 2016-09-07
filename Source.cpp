/* Author:      Vincent Sevilla
 * Filename:    Source.cpp
 * Description: file to drive the hex puzzle program.
 */

#include <iostream>
#include "HexPieces.h"
#include <fstream>

using namespace std;


/*
 * Name:        main 
 * Prototype:   int main(); 
 * Description: This function drives the hex puzzle program 
 * Parameters:  
 *    argc      -Num of parameters, should be 1. 
 *    argv[1]   -represents the duration of a frame (0-5s).
 * Return:      success or failure of execution 
 */
int main(int argc, char * argv[]) {

  HexPieces myPuzzle;
	int possibleSet[ROWSIZE][COLSIZE] = {{0}};
  int originalSet[ROWSIZE][COLSIZE] = {{0}};
  int attempts = 1, showSteps = 0;
 
  if (argc != NUM_OF_ARGS) {
    cout << USAGE_PROMPT; 
    return EXIT_FAILURE;
  }

  try {
    myPuzzle.frameTime = (int)(stod(argv[1], nullptr) * ONE_SECOND);
    if (myPuzzle.frameTime > (5 * ONE_SECOND) || 
        myPuzzle.frameTime < 0) {
      throw 30;
    }
  }
  catch (exception e) {
    cout << USAGE_ERR << USAGE_PROMPT;
    return EXIT_FAILURE;
  }
  catch (int e) {
    cout << USAGE_ERR << USAGE_PROMPT;
    return EXIT_FAILURE;
  }

  myPuzzle.generateARandomSet(possibleSet); 
	while (!myPuzzle.isTheRandomSetSolvable(possibleSet, 0)){
  	myPuzzle.generateARandomSet(possibleSet);
	  attempts++;
	}

  (void)myPuzzle.isTheRandomSetSolvable(possibleSet, 1);

	cout << "The program went through " << attempts <<
      " possible sets of game pieces.\n" << "Set number " << attempts << 
      " is actually solvable." << endl << "Here it is: " << endl;
	
  myPuzzle.displayASet(possibleSet);

	return 0;
}
