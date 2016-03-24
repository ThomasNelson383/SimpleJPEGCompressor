//Thomas Nelson
#include "common.h"

matrix unZigZagMatrix(matrix in) {
	matrix unZigZag;
	unZigZag.initializeV(8, 8);
	
	bool goingDown = false;
	int oldCol = 0;
	int oldRow = 0;
	int newCol = 0;
	int newRow = 0;
	do {
		//The only real difference from ZigZag to this is
		//I switched old with New here
		unZigZag.v[oldRow][oldCol] = in.v[newRow][newCol];
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
	return unZigZag;
}	
DCT getDCTandUnZigZag(char *fileName) {
	std::ifstream file(fileName);
	
	string str = readLineOrSpaceFile(file);	//MYDCT
	if(str.compare("MYDCT") != 0) {
		printf("Didn't have MYDCT\n");
	}
	
	int xSize = getInt(readLineOrSpaceFile(file)); //sizeX
	int ySize = getInt(readLineOrSpaceFile(file)); //sizeY
	float qScale = getFloat(readLineOrSpaceFile(file)); //qScale
	
	DCT dct;
	dct.initializeDCT(xSize, ySize, qScale);
	
	for(int i = 0; i < dct.totalMacroBlocks; ++i) {
		for(int j = 0; j < 4; ++j) {
			int startX = dct.macroBlocks[i].blocks[j].x;
			int startY = dct.macroBlocks[i].blocks[j].y;
			int strStartX = getInt(readLineOrSpaceFile(file)); //sizeX
			int strStartY = getInt(readLineOrSpaceFile(file)); //sizeY
			
			if(startX != strStartX || startY != strStartY) {
				printf("My X or Y didn't match with DCT File");
			}
			
			//Getting the ZigZag matrix
			matrix zigZag;
			zigZag.initializeV(8,8);
			for(int x = 0; x < 8; ++x) {
				for(int y = 0; y < 8; ++y) {
					if(!file.good()) {
						printf("Something happened in getQuantMatrix, whoops\n");
					}
					zigZag.v[x][y] = getInt(readLineOrSpaceFile(file));
				}
			}
			//unZigZag it before storing
			matrix unZigZag = unZigZagMatrix(zigZag);
			//Copying over unZigzag I could pass the refrence, but lazy
			for(int x = 0; x < 8; ++x) {
				for(int y = 0; y < 8; ++y) {
					dct.macroBlocks[i].blocks[j].coefficients.v[x][y] = unZigZag.v[x][y];
				}
			}
		}
	}
	return dct;	
}

PGM unQuantUnDCTgetImage(DCT dct, matrix qunatize, matrixFloat dctCosine) {
	PGM image;
	image.initializePGM(dct.xSize, dct.ySize);
	
	for(int i = 0; i < dct.totalMacroBlocks; ++i) {
		for(int j = 0; j < 4; ++j) {
			int startX = dct.macroBlocks[i].blocks[j].x;
			int startY = dct.macroBlocks[i].blocks[j].y;
			for(int y = 0; y < 8; ++y) {
				for(int x = 0; x < 8; ++x) {
					float sum = 0;
					for(int u = 0; u < 8; ++u) {
						for(int v = 0; v < 8; ++v) {
							//resting the offset
							int quantCoef = dct.macroBlocks[i].blocks[j].coefficients.v[u][v];
							quantCoef -= 127;
							//Multiplying by qunatization
							float DUT  = quantCoef * (qunatize.v[u][v] * dct.Qvalue);

							//Getting the double summing
							sum += 	getC(u) * getC(v) * DUT *
									dctCosine.v[x][v] * dctCosine.v[y][u];
						}
					}
					//Rounding and cropping
					float out = (1/4.0) * sum;
					if(out > 255) {
						out = 255;
					}
					if(out < 0) {
						out = 0;
					}
					image.grayScaleData.v[startY + y][startX + x] = (int)round(out);		
				}
			}
		}
	}
	return image;
}

void printToFile(char *fileName, PGM out) {
	ofstream  outfile;
	outfile.open(fileName, ios::binary | ios::out);
	string s = out.toString();
	outfile.write(s.c_str(), s.size());
	outfile.close();	
}
int main(int argc, char *argv[]) {
    if(argc != 4) {
		printf("Need 3 aggruments\n");
		return -1;
	}
	//getting cosine
	matrixFloat dctCosine = getDCTCosine();
	//Getting the DCT
	DCT inDCT = getDCTandUnZigZag(argv[1]);
	//printf("%s", inDCT.toString().c_str());
	//Getting the qunatMatrix
	matrix quantMatrix = getQuantMatrix(argv[2]);
	//for(int x = 0; x < 8; ++x) {
	//	for(int y = 0; y < 8; ++y) {
	//		printf("%5d", (int)(quantMatrix.v[x][y]));
	//	}
	//	printf("\n");
	//}
	//Getting the IMG
	PGM outImage = unQuantUnDCTgetImage(inDCT, quantMatrix, dctCosine);	
	//printf("%s", outImage.toNiceString().c_str());
	//printf("\nnext\n");
	//print to file
	printToFile(argv[3], outImage);
	
	
	return 0;
}
