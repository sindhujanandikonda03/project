/* This decompressor will take 3 input parameters:
        myIDCT <DCT file of image> <quantfile> <output image(PGM)>
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* read quantfile, split each line by white space and store values into matrix */
void build_quantMatrix(FILE *file, int size, int quantMatrix[size][size]){
    char line[40];
    int len, line_num=0, i=0, j;
    char *token;
    int tmp;
    while(fgets (line, sizeof(line), file) != NULL){
	len = strlen(line);
        if(line[len-1] == '\n')	 line[len-1]='\0';
	token = strtok(line, " ");
        i=0;
	while(token != NULL){
	   tmp = atoi(token);
	   quantMatrix[line_num][i]=tmp;
	   token = strtok(NULL, " ");
	   i++;
        }
	line_num++;
    }
}
/* read data from DCT file and build 8 by 8 coefficients matrix (8 lines each time) */
void dctToMatrix(FILE *file, int coeff[8][8]){
    char line[400];
    int len, line_num=0, i=0, j;
    char *token;
    int tmp;
    while(fgets (line, sizeof(line), file) != NULL){
	len = strlen(line);
        if(line[len-1] == '\n')	 line[len-1]='\0';
	token = strtok(line, " ");
        i=0;
	while(token != NULL){
	   coeff[line_num][i]=atoi(token);
	   token = strtok(NULL, " ");
	   i++;
        }
	line_num++;
	if(line_num == 8) break;

    }
}

static float C(int val){
  if(val == 0){
    return 1.0 / sqrt(2.0);
  }else{
    return 1.0;
   }
}

void idctTransform(double coeff[8][8], int idctMatrix[8][8]){
	int u, v, x, y;
	double cv, cu;
	double t, temp;
        double tmpMatrix[8][8];
	for(x=0; x<8; x++)
	  for(y=0; y<8; y++){
	     temp = 0;
	     tmpMatrix[x][y] = 0;

	     for(u=0;u<8;u++){
		for(v=0;v<8;v++){
		  
		  temp += coeff[v][u]* C(u) * C(v) * cos((v * M_PI * (2*x+1))/16)*cos((u*M_PI*(2*y+1))/16);	
		  
		}
 	     }
	      tmpMatrix[x][y] = temp * 0.25;
          }
	
        for(x=0; x<8; x++)
	       for(y=0; y<8; y++){
                 if(tmpMatrix[x][y] < 0)  tmpMatrix[x][y] = 0;
                 if(tmpMatrix[x][y] > 255)  tmpMatrix[x][y] = 255;
		 //idctMatrix[x][y] = round(tmpMatrix[x][y]);
	         idctMatrix[x][y] = (int)(tmpMatrix[x][y]);
              }
}

/* Resetting the offset of range to [-127,128] and multiply with quantization matrix */
void deQuantization(int coeff[8][8], int quantMatrix[8][8], double results[8][8], double qscale){
	int x, y;
	for(x=0; x<8; x++)
	  for(y=0; y<8; y++){
	    coeff[x][y] -= 127;
	    //results[x][y]= (double)coeff[x][y] * (double)quantMatrix[x][y] * (double)qscale;
	    results[x][y]= coeff[x][y] * quantMatrix[x][y] * qscale;
	  }
}
/* copy 8*8 matrix to 16*16 matrix */
void copyTo16by16Matrix(int matrix[8][8], int x, int y, int output[16][16]){
	int i, j;
        for(i=0; i<8; i++)
	  for(j=0; j<8; j++){
	     output[x+i][y+j] = matrix[i][j];
          }
}

void writeToBuffer(int block[16][16], int start, int size, int data_block[]){
	int i, j, k=0;

	for(i=start;i<16*size;i+=size){
	   for(j=0;j<16;j++){
		data_block[i+j] = block[k][j];
	        //printf(" %d ", data_block[i+j]);
	   }
	   k++;
	   if(k==16) break;
	}
}

int main(int argc, char** argv)
{
int i = 0, j,m=0,n;;
int idx=0, num=0, num_block16=0;
double qvalue;
char line[50];
char *token;
int quantMatrix[8][8], coordinates[2], coeff[8][8], idctMatrix[8][8], size[2];
int microBlock[16][16], num_block_x=0, num_block_y=0;
double results[8][8];
unsigned char val;
FILE *output, *in_dct, *quantfile;

quantfile = fopen(argv[2], "r");
build_quantMatrix(quantfile, 8, quantMatrix);

in_dct = fopen(argv[1], "r");
output = fopen(argv[3],"w+");

fgets(line, sizeof(line), in_dct);
fprintf(output, "P5\n");
fgets(line, sizeof(line), in_dct);   // Get xsize ysize
fprintf(output, "%s", line);
token = strtok(line, " ");
i=0; 
while(token != NULL){
  size[i]=atoi(token);
  token = strtok(NULL, " ");
  i++;
}

fprintf(output, "255\n");
fgets(line, sizeof(line), in_dct);   //qscale
qvalue = atof(line);

int data_block[16*size[1]];

while(1){

   if(fgets (line, sizeof(line), in_dct) == NULL)  break;
   //printf("%s", line);
   token = strtok(line, " ");
   idx=0; 
   while(token != NULL){
	   coordinates[idx]=atoi(token);
	   token = strtok(NULL, " ");
	   idx++;
   }

   dctToMatrix(in_dct, coeff);
   deQuantization(coeff, quantMatrix, results, qvalue);
   
   idctTransform(results, idctMatrix);
     
   copyTo16by16Matrix(idctMatrix, coordinates[1]-16*num_block_y, coordinates[0]-16*num_block_x, microBlock);
   num++;
   /* Every 4 8by8 matrix to 16 by 16 and write out all bytes of 16 by 16 matrix */
   if(num%4 == 0){
	num_block_x++;
	if (num_block_x >= size[0]/16) { num_block_x = 0; num_block_y++; }
	num_block16++;
	writeToBuffer(microBlock, 16*(num_block16-1), size[1], data_block);
 	if(num_block16 == 16*size[1]/256){
		num_block16=0;
		for(m=0; m<16*size[1]; m++){
		   //printf(" %d ", data_block[m]);
		   //if(m%512 == 0)   printf("\n\n\n\n");
		   val = data_block[m];
		   fwrite(&val, sizeof(char), 1, output);
		}
	}
   }
}

fclose(quantfile);
fclose(in_dct);
fclose(output);

return 0;

}
