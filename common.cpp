//Thomas Nelson
#include "common.h"

//Converts str to a number
int getInt(string str) {
	int result = 0;
	stringstream convert(str);
	convert >> result;
	return result;
}
float getFloat(string str) {
	float result = 0;
	stringstream convert(str);
	convert >> result;
	return result;
}
//get the quantMatrix
matrix getQuantMatrix(char* fileName) {	
	matrix quantMatrix;
	quantMatrix.initializeV(8,8);
	
	std::ifstream file(fileName);
	
	for(int x = 0; x < 8; ++x) {
		for(int y = 0; y < 8; ++y) {
			if(!file.good()) {
				printf("Something happened in getQuantMatrix, whoops\n");
			}
			
			quantMatrix.v[x][y] = getInt(readLineOrSpaceFile(file));
		}
	}
	return quantMatrix;
}
//to see each "word"
string readLineOrSpaceFile(std::ifstream &fileStream) {
	string str;
	
	streambuf *sb = fileStream.rdbuf();
	
	bool something = false;	//Used to make sure I don't return a blank, it waits until I get something back
	while(true) {
		char appendChar = sb->sbumpc();
		//printf("Thinking of adding: %d\n", (int)appendChar);
		switch(appendChar) {
			case '\n':
			case ' ':
			case '\r':
				if(something){
					return str;
				} 
				break;
			case EOF:
				return str;
			default:
				//printf("Adding: %c\n", appendChar);
				something = true;
				str += appendChar;	
		}
	}
	return str;
}
//The Cu or Cv for DCT
double getC(int uv) {
	if(uv == 0)
		return 1.0/sqrt(2.0);
	else 
		return 1.0;
}
//Static away of having the cosine data, to be a little quicker
matrixFloat getDCTCosine() {
	matrixFloat dctCosine;
	dctCosine.initializeV(8,  8);
	for(int v = 0; v < 8; ++v) {
		for(int x = 0; x < 8; ++x) {
			dctCosine.v[x][v] = cos(((2 * x + 1) * v * M_PI) / 16.0);
		}
	}
	return dctCosine;
}

//matrixs - could have used polyphenism 
void matrix::initializeV(int _row, int _column) {
	row = _row;
	column = _column;
	v.resize(row);
	for(int i = 0; i < row; ++i) {
		v[i].resize(column);
	}
	
	clear();
}
void matrix::clear() {
	for(int i = 0; i < row; ++i) {
		for(int j = 0; j < column; ++j) {
			v[i][j] = 0;
		}
	}
}
void matrixFloat::initializeV(int _row, int _column) {
	row = _row;
	column = _column;
	v.resize(row);
	for(int i = 0; i < row; ++i) {
		v[i].resize(column);
	}
	
	clear();
}
void matrixFloat::clear() {
	for(int i = 0; i < row; ++i) {
		for(int j = 0; j < column; ++j) {
			v[i][j] = 0;
		}
	}
}

//PGM	
void PGM::initializePGM(int _xSize, int _ySize) {
	xSize = _xSize;
	ySize = _ySize;
		
	grayScaleData.initializeV(ySize, xSize);
}
string PGM::toNiceString() {
	stringstream out;
	out << xSize << " " << ySize << "\n";
	for(int y = 0; y < ySize; ++y) {
		for(int x = 0; x < xSize; ++x) {
			out.width(5);
			out << grayScaleData.v[y][x];
		}
		out << "\n";
	}
	return out.str();
}
string PGM::toString() {
	stringstream out;
	out << "P5\n";
	out << xSize << " " << ySize << "\n";
	out << "255\n";
	for(int y = 0; y < ySize; ++y) {
		for(int x = 0; x < xSize; ++x) {
			out << (unsigned char)grayScaleData.v[y][x];
		}
	}
	return out.str();
}
//Block
matrix zigZag(matrix coefficients) {
	matrix zigZag;
	zigZag.initializeV(8, 8);
	
	bool goingDown = false;
	int oldCol = 0;
	int oldRow = 0;
	int newCol = 0;
	int newRow = 0;
	do {
		zigZag.v[newRow][newCol] = coefficients.v[oldRow][oldCol];
		if(newCol == 7) {
			newRow += 1;
			newCol = 0;
		} else {
			newCol += 1;
		}
		//Setting up for the next one
		if(goingDown) {
			if(oldCol == 0 || oldRow == 7) {
				//Turning around
				goingDown = false;
				if(oldRow > 7 || oldRow + 1 > 7) {
					//Button half
					oldCol += 1;
					oldRow = 7;
				} else {
					//left half
					oldCol = 0;
					oldRow += 1;
				}					
			} else {
				oldCol -= 1;
				oldRow += 1;
			}
		} else {
			if(oldRow == 0 || oldCol == 7) {
				//turning around
				goingDown = true;
				if(oldCol == 7) {
					//right half
					oldCol = 7;
					oldRow += 1;
				} else {
					//top half
					oldRow = 0;
					oldCol += 1;
				}
			} else {
				oldRow -= 1;
				oldCol += 1;
			}
		}
	} while(oldCol != 8 && oldRow != 8);
	return zigZag;
}
void block::initializeBlock(int _x, int _y) {
	x = _x;
	y = _y;
	//printf("x: %d, y: %d\n", x, y);		
	coefficients.initializeV(8, 8);
}

string block::toString() {
	stringstream out;
	matrix zigZagMatrix = zigZag(coefficients);
	//printf("x: %d, y: %d\n", x, y);
	out << x << " " << y << "\n";
	for(int x = 0; x < 8; ++x) {
		for(int y = 0; y < 8; ++y) {
			out.width(5);
			out << zigZagMatrix.v[x][y];
		}
		out << "\n";
	}
	return out.str();
}

//macroBlock
void macroBlock::initializeMacroBlock(int x, int y) {
	blocks[0].initializeBlock(x,   y);
	blocks[1].initializeBlock(x+8, y);
	blocks[2].initializeBlock(x,   y+8);
	blocks[3].initializeBlock(x+8, y+8);
}
string macroBlock::toString() {
	stringstream out;
	for(int i = 0; i < 4; ++i){
		out << blocks[i].toString();
	}
	return out.str();
}

//DCT
void DCT::initializeDCT(int _xSize, int _ySize, float _Qvalue) {
	xSize = _xSize;
	ySize = _ySize;
	Qvalue = _Qvalue;
	
	totalMacroBlocks = _xSize/16 * _ySize/16;
	
	for(int i = 0; i < totalMacroBlocks; ++i) {
		int x = (i * 16) % xSize;
		int y = i/(xSize/16) * 16;
		macroBlocks[i].initializeMacroBlock(x, y);
	}
		
}

string DCT::toString() {
	stringstream out;
	out << "MYDCT\n";
	out << xSize << " " << ySize << "\n";
	out.precision(6);
	out << fixed << Qvalue << "\n";
	for(int i = 0; i < totalMacroBlocks; ++i) {
		out << macroBlocks[i].toString();
	}
	return out.str();
}
