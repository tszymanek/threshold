//Tomasz Szymanek
//Zaawansowane języki programowania
//*** Zadania poprawkowe
//Zadanie 1 (Obróbka grafiki: filtr Threshold)
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void malloc_macierz(int **M, int w, int h){
	*M=malloc((w * h) * sizeof(int));
}

void malloc_podmacierz(int **M, int w, int h, int l_procesow){
	*M=malloc(((w * h) / l_procesow) * sizeof(int));
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

void thresh_macierz(int *M, int *m, int w, int h, int l_procesow, int THRESH){
	int i, j, k;
	if(((w*h) % l_procesow)==0){
		int lpnp=(int)((w*h)/l_procesow); //liczba_pikseli_na_proces
		for(i=0;i<l_procesow;i++){
			for(j=i*lpnp, k=0;j<lpnp + i*lpnp;j++, k++)
				m[k]=M[j];	//symulacja dzielenia macierzy
			//symulacja rozsyłania podmacierzy do wszystkich procesów
			for(k=0;k<lpnp; k++){
				if(m[k]>=THRESH)	//symulacja operacji Threshold
					m[k]=255;
				else
					m[k]=0;
			}
			//symulacja wysyłania podmacierzy do procesu 0
			for(j=i*lpnp, k=0;j<lpnp + i*lpnp;j++, k++)
				M[j]=m[k];	//symulacja łączenia podmacierzy w macierz
		}
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
	int *M, *m;				//macierz, podmacierz
	const int w=640, h=480;	//szerokosc, wysokosc
	int THRESH=100;
	//scanf("%d", &THRESH);
	clock_gettime(CLOCK_REALTIME, &t1);
	int liczba_procesow=4;	//, numer_procesu;
	malloc_macierz(&M, w, h);
	malloc_podmacierz(&m, w, h, liczba_procesow);
	generuj_macierz(M, w, h);
	zapisz_do_pliku("monochrome_s.ppm", M, w, h);
	//wypisz_macierz(M, w, h);
	//printf("\n");
	thresh_macierz(M, m, w, h, liczba_procesow, THRESH);
	zapisz_do_pliku("threshold_s.ppm", M, w, h);
	//wypisz_macierz(M, w, h);
	clock_gettime(CLOCK_REALTIME, &t2);
	long nansec = t2.tv_nsec - t1.tv_nsec; 
	printf("Czas wykonywania programu wyniosl:\t%ld nanosekund\n", nansec);
	return 0;
}
