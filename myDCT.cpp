//Thomas Nelson
#include "common.h"

PGM getImageFile(char* fileName) {
	std::ifstream file(fileName);
	
	string str = readLineOrSpaceFile(file); //'P5'
	//printf("P5? %s\n", str.c_str());
	if(str.compare("P5") != 0) {
		printf("The image file didn't have P5\n");
	}
	int xSize = getInt(readLineOrSpaceFile(file)); //sizeX
	int ySize = getInt(readLineOrSpaceFile(file)); //sizeY
	
	str = readLineOrSpaceFile(file); //255
	if(str.compare("255") != 0) {
		printf("The image file didn't have 255\n");
	}
	PGM image;
	image.initializePGM(xSize, ySize);
	for(int y = 0; y < ySize; ++y) {
		for(int x = 0; x < xSize; ++x) {
			unsigned char asciiChar;
			
			if(!file.good()) {
				printf("Something happened in getImageFile\n");
				return image;
			} else {
				file >> std::noskipws >> asciiChar;	//noskipws makes sure I get the spaces and new lines, for they are data
				image.grayScaleData.v[y][x] = int(asciiChar);
			}
		}
	}
	return image;
}
DCT getDCTFromPGMandQuantize(PGM image, float qScale, matrix qunatize, matrixFloat dctCosine) {
	DCT dct;
	dct.initializeDCT(image.xSize, image.ySize, qScale);
	
	for(int i = 0; i < dct.totalMacroBlocks; ++i) {
		for(int j = 0; j < 4; ++j) {
			int startX = dct.macroBlocks[i].blocks[j].x;
			int startY = dct.macroBlocks[i].blocks[j].y;
			for(int u = 0; u < 8; ++u) {
				for(int v = 0; v < 8; ++v) {
					//Getting the double summing
					float sum = 0;
					for(int y = 0; y < 8; ++y) {
						for(int x = 0; x < 8; ++x) {
							sum += 	image.grayScaleData.v[startY + y][startX + x] *
									dctCosine.v[x][v] * dctCosine.v[y][u];
						}
					}
					float DUT = getC(u)/2.0 * getC(v)/2.0 * sum;
					//qunatize
					float quantCoefD = DUT / (qunatize.v[u][v] * dct.Qvalue);
					int quantCoef = (int)round(quantCoefD);
					if(quantCoef > 128) {
						quantCoef = 128;
					} else if(quantCoef < -127) {
						quantCoef = -127;
					}
					quantCoef += 127;
					dct.macroBlocks[i].blocks[j].coefficients.v[u][v] = quantCoef;
				}
			}
		}
	}
	return dct;
}
void printToFile(char *fileName, DCT out) {
	FILE* file = fopen(fileName, "wb");	
	
	fputs(out.toString().c_str(), file);

	fclose(file);
}
int main(int argc, char *argv[]) {
    if(argc != 5) {
		printf("Need 4 aggruments\n");
		return -1;
	}
	matrixFloat dctCosine = getDCTCosine();
	//Getting the Image File into the PGM Struct
	PGM imagePGM = getImageFile(argv[1]);
	//printf("%s", imagePGM.toString().c_str());
	
	//Getting the qunatMatrix
	matrix quantMatrix = getQuantMatrix(argv[2]);
	//for(int x = 0; x < 8; ++x) {
	//	for(int y = 0; y < 8; ++y) {
	//		printf("%5d", (int)(quantMatrix.v[x][y]));
	//	}
	//	printf("\n");
	//}
	
	//Storing the qScale
	float qScale = getFloat(argv[3]);
	//printf("%.1f\n", qScale);
	
	//Getting the DCT output
	DCT out = getDCTFromPGMandQuantize(imagePGM, qScale, quantMatrix, dctCosine);
	//printf("%s", out.toString().c_str());
	
	//print to file
	printToFile(argv[4], out);

	return 0;
}
