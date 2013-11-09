//Tomasz Szymanek
//Zaawansowane języki programowania
//*** Zadania poprawkowe
//Zadanie 1 (Obróbka grafiki: filtr Threshold)
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h"

void malloc_macierz(int **M, int w, int h){
	*M=malloc((w * h) * sizeof(int));
}

void malloc_podmacierz(int **m, int w, int h, int l_procesow){
	*m=malloc(((w * h) / l_procesow) * sizeof(int));
}

void generuj_macierz(int *M, int w, int h){
	int i, j;
	for(i=0;i<h;i++){
		for(j=0;j<w;j++)
			M[j+i*w]=rand()%256;
	}
}

void wypisz_macierz(int *M, int w, int h){
	int i, j;
	for(i=0;i<h;i++){
		for(j=0;j<w;j++)
			printf("%d \t", M[j+i*w]);
		printf("\n");
	}
}

void dziel_i_rozsylaj_macierz(int *M, int *m, int w, int h, int l_procesow){
	if(((w*h) % l_procesow)==0){
		int lpnp=(int)((w*h)/l_procesow); //liczba_pikseli_na_proces
		MPI_Scatter(M, lpnp, MPI_INT, m, lpnp, MPI_INT, 0, MPI_COMM_WORLD);
	}
	else
		printf("Macierz nie jest proporcjonalnie podzielna.");
}

void thresh_i_wysylaj_podmacierz(int *m, int *M, int w, int h, int l_procesow, int THRESH){
	int i;
	if(((w*h) % l_procesow)==0){
		int lpnp=(int)((w*h)/l_procesow); //liczba_pikseli_na_proces
			for(i=0;i<lpnp; i++){
				if(m[i]>=THRESH)
					m[i]=255;
				else
					m[i]=0;
			}
		MPI_Gather(m, lpnp, MPI_INT, M, lpnp, MPI_INT, 0, MPI_COMM_WORLD);
	}
	else
		printf("Macierz nie jest proporcjonalnie podzielna.");
}

void zapisz_do_pliku(char *nazwa_pliku, int *M, int w, int h){
	int i, j;
	FILE *plik = fopen(nazwa_pliku, "wb");
	(void) fprintf(plik, "P6\n%d %d\n255\n", w, h);
	for(i=0;i<h;i++){
		for(j=0;j<w;j++){
			static unsigned char kolor[3];
			kolor[0]=M[j+i*w];
			kolor[1]=M[j+i*w];
			kolor[2]=M[j+i*w];
			(void) fwrite(kolor, 1, 3, plik);
		}
	}
}

int main(int argc, char **argv){
	struct timespec t1, t2;
	int liczba_procesow, numer_procesu;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &numer_procesu);
	MPI_Comm_size(MPI_COMM_WORLD, &liczba_procesow);
	
	int *M, *m;				//macierz, podmacierz
	const int w=640, h=480;	//szerokosc, wysokosc
	int THRESH;

	if(numer_procesu==0){
		clock_gettime(CLOCK_REALTIME, &t1);
		malloc_macierz(&M, w, h);
		generuj_macierz(M, w, h);
		//wypisz_macierz(M, w, h);
		//printf("\n");
		zapisz_do_pliku("monochrome_p.ppm", M, w, h);
	}
	
	malloc_podmacierz(&m, w, h, liczba_procesow);
	dziel_i_rozsylaj_macierz(M, m, w, h, liczba_procesow);

	if(numer_procesu==0){
		//scanf("%d", &THRESH);
		THRESH=100;
	}
	
	MPI_Bcast(&THRESH, 1, MPI_INT, 0, MPI_COMM_WORLD);
	thresh_i_wysylaj_podmacierz(m, M, w, h, liczba_procesow, THRESH);
	
	if(numer_procesu==0){
		//wypisz_macierz(M, w, h);
		zapisz_do_pliku("threshold_p.ppm", M, w, h);
		clock_gettime(CLOCK_REALTIME, &t2);
		long nansec = t2.tv_nsec - t1.tv_nsec; 
		printf("Czas wykonywania programu wyniosl:\t%ld nanosekund\n", nansec);
	}
	MPI_Finalize();
	return 0;
}
