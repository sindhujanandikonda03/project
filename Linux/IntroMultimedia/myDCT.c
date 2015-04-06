/* This program is implementing the DCT transform and Quantization for a grayscale image, which will take 4 
   input parameters:
      myDCT <PGM image> <quantfile> <qscale> <output file>
 
      Image Format(PGM):
         P5\n
	 <xsize> <ysize>\n
	 255\n
	 [xsize*ysize bytes of grayscale data, left to right, top to bottom]
      Compressed File Format:
	 MYDCT\n
	 <xsize> <ysize>\n		
	 Qvalue\n
	 [xsize/16 * ysize/16 blocks of DCTcoefficients]

	 Each block will be encoded into the file as:
	   x_offset(in pixels) y_offset(in pixels)
	   DCT values 8 in a line with 8 rows
         xsize and ysize will be multiples of 16
*/

#include <stdio.h>
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

void macroBlock16by16(int dataBlock[], int start, int size, int macroBlock[16][16]){
	int i, j, k=0;
	for(i=start; i < 16 *size; i+=size){
	   for(j=0;j<16;j++){
	      macroBlock[k][j] = dataBlock[i+j];
	   }
	   k++;
	}
	printf("\n	16  16 		\n");
	for(i=0;i<16;i++){
	  for(j=0;j<16;j++)
	     printf(" %d ", macroBlock[i][j]);
	  printf("\n");
	}
}
/* Extract the 8 by 8 matrix from 16 by 16 */
void convertTo8by8(int macroBlock[16][16], int x, int y, int block[8][8]){
	int i,j;
	for(i=0; i< 8; i++){
	  for(j=0; j< 8; j++)
	     block[i][j] = macroBlock[i+x][y+j];
	}

	printf("\n------------------------8 by 8--------------------\n");
	for(i=0; i< 8; i++){
	  for(j=0; j< 8; j++)
		printf(" %d ", block[i][j]);
	  printf("\n");
	}  
}

static double C(int val){
   if(val == 0)
	return 1.0 / sqrt(2.0);
   else
	return 1.0;
}

void dctTransform(int matrix[8][8], double dctMatrix[8][8]){
	int u, v, x, y;
	double temp; 

	for(u=0; u<8; u++)
	  for(v=0; v<8; v++){
	     
	     temp = 0;
	     dctMatrix[u][v] = 0;
		
	     for(x=0;x<8;x++){
		for(y=0;y<8;y++){
		  temp += matrix[x][y]*cos(((2*x+1)*u*M_PI) / 16)*cos(((2*y+1)*v*M_PI) / 16);
		}
 	     }
	     dctMatrix[u][v] = C(u) * C(v) * 0.25 * temp;
          }

	/*printf("\n---- ---- DCT ----- -------\n");
	for(x=0;x<8;x++){
		for(y=0;y<8;y++)
		    printf("  %f  ", dctMatrix[x][y]);
	printf("\n");		
	} */
}

/* Quantization and cropping, rounding coefficients to the range [0,255] */
void quantization(double dctMatrix[8][8], int quantMatrix[8][8], int quantcoeff[8][8], int qscale){
	int x, y;

	for(x=0; x<8; x++){
	  for(y=0; y<8; y++){
	    //quantcoeff[x][y] = 0;
	    quantcoeff[x][y] = round(dctMatrix[x][y]/(quantMatrix[x][y] * qscale));

	    if(quantcoeff[x][y]>128) quantcoeff[x][y] = 128;
	    if(quantcoeff[x][y]<-127) quantcoeff[x][y]= -127;
	    quantcoeff[x][y] += 127;
	  }
        }
}

int main(int argc, char** argv)
{
int i = 0, j, k=0,m,n, idx=0, num, data_start, x, y;
char line[32];

char *token;
int macroBlock[16][16], quantMatrix[8][8], block[8][8], quantcoeff[8][8], size[2], idx_block=0, idy_block=0;
int qvalue;
double dctMatrix[8][8];
unsigned char byte;
FILE *output, *in_image, *quantfile;

quantfile = fopen(argv[2], "r");
build_quantMatrix(quantfile, 8, quantMatrix);

in_image = fopen(argv[1], "r");

qvalue = atoi(argv[3]);
output = fopen(argv[4],"w+");
fprintf(output, "MYDCT\n");

fgets(line, sizeof(line), in_image);   // Get "P5"
int t1=strlen(line);
fgets(line, sizeof(line), in_image);   // Get xsize ysize
int t2=strlen(line);
fprintf(output, "%s", line);
token = strtok(line, " ");
i=0; 
while(token != NULL){
  size[i]=atoi(token);
  token = strtok(NULL, " ");
  i++;
}

fprintf(output, "%f\n", (double)qvalue);

data_start = t1+t2+4;
//fseek(in_image, data_start, SEEK_SET);  // ignore the header bytes

int dataBlock[16*size[1]];

num = size[0] / 16;
// 16*16   -- 8*size[1]/256 1(x)   
while(k < num){
	fseek(in_image, data_start + k*16*size[1], SEEK_SET);
	while(fread(&byte, 1, 1, in_image) == 1){
		dataBlock[idx++] = (int)byte;
		if(idx==16*size[1]) { idx=0; break; }
	}
	printf("\n	data 	\n");	
	for(i=0; i<16*size[1];i++){
   		printf(" %d ", dataBlock[i]);
		if(i % 512 == 0)  printf("\n\n\n\n\n");
	}
	printf("\n	data 	\n");
	for(i=0; i< size[1]/16; i++){
		macroBlock16by16(dataBlock, 16*i, size[1], macroBlock);
		for(x=0;x<2;x++)
		   for(y=0;y<2;y++){
			fprintf(output, "%d %d\n", 16*idx_block+8*y, 16*idy_block+8*x);
		  	convertTo8by8(macroBlock, 8*x, 8*y, block);
			dctTransform(block, dctMatrix);
			quantization(dctMatrix, quantMatrix, quantcoeff, qvalue);
 			for(m=0; m< 8; m++){
		  		for(n=0; n< 8; n++){
		     			printf(" %d", quantcoeff[m][n]);
                     			fprintf(output, " %d", quantcoeff[m][n]);
		  		}
		  	printf("\n");
		  	fprintf(output, "\n");
			}
		}
		idx_block++;
		if(16*idx_block+8 >= size[0]) { idx_block=0; idy_block++;}
	}
	k++;
	//break;
}
fclose(quantfile);
fclose(in_image);
fclose(output);

return 0;

}
