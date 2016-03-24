//Thomas Nelson
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <math.h>
#include <sstream>
#include <fstream>

using namespace std;

//basic 2D matrix int class
//	To store the Ints in a 2D array easily
struct matrix {
	int row;
    int column;
    vector< std::vector<int> > v;
	
	void initializeV(int _row, int _column);
	void clear();
};
//basic 2D matrix int class
//	To store the doubles in a 2D array easily
struct matrixFloat {
	int row;
    int column;
    vector< std::vector<double> > v;
	
	void initializeV(int _row, int _column);
	void clear();
};
//image class
struct PGM {
	int xSize;
	int ySize;
	matrix grayScaleData; //[y][x]
	
	void initializePGM(int _xSize, int _ySize);
	
	string toNiceString();		//To int X x Y gride
	string toString();			//To bytes
};
//a block worth of data 8x8 Y DCT data
struct block {
	int x;				  //The top right location of the 8x8
	int y;
	matrix coefficients; //[8][8]
	
	void initializeBlock(int _x, int _y);	//Gives an 8x8 coefficient matrix with top right location
	string toString();	//Gives the xy location and an 8x8 grid of ints
};
//macroBlock worth of data 4 8x8 Y DCT data
struct macroBlock {
	block blocks[4]; //4 blocks in a macro block

	void initializeMacroBlock(int x, int y);	//Gives 4 blocks with the start location top right
	string toString();	//prints each block
};
//Full DCT data
struct DCT {
	int xSize;	//multiple of 16
	int ySize;  //multiple of 16
	float Qvalue;
	macroBlock macroBlocks[(640/16) * (480/16)];  //size is x/16 * y/16
	int totalMacroBlocks;		//How many macroblocks there are in the DCT should never have more than 1200
	
	void initializeDCT(int _xSize, int _ySize, float _Qvalue);
	string toString();	//prints the DCT into the nice formating. It goes through each macroblock
};


//Const DCT cosine to store and 
//used later to run a little after
matrixFloat getDCTCosine();
//to get the C for the DCT
double getC(int uv);

//Convert from string functions
int getInt(string str);
float getFloat(string str);

//get number to a number in a file
string readLineOrSpaceFile(std::ifstream &fileStream);

//to get the quantmatrix from a file
matrix getQuantMatrix(char* fileName);

