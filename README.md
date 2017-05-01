#### Description:
##### DCT:
This program takes a pgm file and creates a DCT file. It does this by takes the pgm and creating a 2D matrix of ints it to easily use later, this is the PGM data structure. It then grabs the quantMatrix file and then again creates a 2D matrix of ints to be easily used later; I only used the matrix structure. Then I create a mockup of what the DCT should be, but take the width and height of the image by initialize the DCT data structure. This DCT structure as microblocks which are 16x16 pix, and then is broken down to blocks with 8x8 pix, which is a matrix that stores the x and y coordinates of the section it will convert. I also create a “static” matrix of the cosine data for it will speed the process up a tad. I think put the quantMatrix, the DCT, and the PGM structs into the DCT function. After I am done with the DCT I zigzag the DCT and output the string.

    ./myDCT <pgmFile> <quantFile> <quantLevel> <outPutDCTFile>
##### IDCT:
This program takes a DCT file and output a PGM file. This program is very similar as the one above, but reverse. I got the DCT cosine data in a matrix. I then got the DCT into the structure, but while I was getting the 8 by 8 matrix I unzigzaged it before I put it into the DCT structure.  I got the quantMatrix into the 2D matrix again. Then I took the DCT and the quantMatrix into function of the IDCT and output the PGM file I wanted. Then I got the string of the PGM and outputted the file. 

    ./myIDCT <DCTFile> <quantFile> <outPutPGMFile>
#### What works:
Everything should work as stated in the assignment to do
##### DCT:
*	Getting the image into my program
*	Getting the quantMatrix into my program
*	Perform the DCT
*	Perform the Quant
*	Perform the Zigzag
*	Print the DCT with the zigzag

##### IDCT
*	Getting the DCT into my program
*	Perform the unzigzag
*	Getting the quantMatrix into my program
*	Perform the unQuantize
*	Perform the unDCT
*	Printing the image

#### What doesn’t work:
There shouldn’t be anything that doesn’t work
#### What was hard:
I had a big problem with floating point off by one. I corrected this by add .000001 to the number so it can round, but sounds like you are alright with leaving this out. I also had a problem with my xs and ys. I thought X was the “height” and not the “width” This was a problem for a while, until I quickly switched. 

#### What kinds of testing I did:
*	I first printed out all the files I was getting to make sure I got everything and correctly
*	I made sure my DCT cosine was getting the correct output by using excel

##### DCT:
*	Ran the DCT, and use excel to compare my values with it
*	Ran the quantize, and again compare the results with excel
*	Ran the Zigzag, print out the results and compared them with yours

##### IDCT:
*	Ran the unzigzag and compared with unzigzag of DCT
*	Ran the quantize and compared it with excel
*	Ran the  IDCT and compared it with excel
*	Print out the image and visualized what I got

Then I did a few of my own simple ones
##### EXAMPLE computer with quantize 1:
###### Before:
![alt text](https://github.com/ThomasNelson383/SimpleJPEGCompressor/blob/master/Examples/computerBefore.gif)
###### After:
![alt text](https://github.com/ThomasNelson383/SimpleJPEGCompressor/blob/master/Examples/computerAfter.gif)
##### EXAMPLE Lena with quantize 5.3:
###### Before:
![alt text](https://github.com/ThomasNelson383/SimpleJPEGCompressor/blob/master/Examples/lenaBefore.gif)
###### After:
![alt text](https://github.com/ThomasNelson383/SimpleJPEGCompressor/blob/master/Examples/lenaAfter.gif)
##### EXAMPLE TOWER quantize 12
###### Before:
![alt text](https://github.com/ThomasNelson383/SimpleJPEGCompressor/blob/master/Examples/towerBefore.gif)
###### After:
![alt text](https://github.com/ThomasNelson383/SimpleJPEGCompressor/blob/master/Examples/towerAfter.gif)
