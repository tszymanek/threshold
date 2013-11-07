#include <stdio.h>
#include <stdlib.h>



int main(int argc, char *argv[]){
	int *M;
	int w=6, h=5;	//w, h;
	int i, j;
	M=malloc(640*480*sizeof(int));
	for(i=0;i<h;i++){
		for(j=0;j<w;j++)
			M[i*h+j]=rand()%256;
	}
	for(i=0;i<h;i++){
		for(j=0;j<w;j++)
			printf("%d \t", M[i*h+j]);
		printf("\n");
	}
	return 0;
}
