//Tomasz Szymanek
//Zaawansowane języki programowania
//*** Zadania poprawkowe
//Zadanie 1 (Obróbka grafiki: filtr Threshold)
#include <stdio.h>
#include <stdlib.h>

void malloc_macierz(int **macierz, int szerokosc, int wysokosc){
	*macierz=malloc((szerokosc * wysokosc) * sizeof(int));
}

void malloc_podmacierz(int **macierz, int szerokosc, int wysokosc, int l_procesow){
	*macierz=malloc(((szerokosc * wysokosc) / l_procesow) * sizeof(int));
}

void generuj_macierz(int *macierz, int szerokosc, int wysokosc){
	int i, j;
	for(i=0;i<wysokosc;i++){
		for(j=0;j<szerokosc;j++)
			macierz[j+i*szerokosc]=rand()%256;
	}
}

void wypisz_macierz(int *macierz, int szerokosc, int wysokosc){
	int i, j;
	for(i=0;i<wysokosc;i++){
		for(j=0;j<szerokosc;j++)
			printf("%d \t", macierz[j+i*szerokosc]);
		printf("\n");
	}
}

void dziel_macierz(int *macierz, int *podmacierz, int szerokosc, int wysokosc, int liczba_procesow){
	int i, j, k;
	if(((szerokosc*wysokosc) % liczba_procesow)==0){
		int l_pikseli_na_proces=(int)((szerokosc*wysokosc)/liczba_procesow);
		for(i=0;i<liczba_procesow;i++){
			for(j=i*l_pikseli_na_proces, k=0;j<l_pikseli_na_proces + i*l_pikseli_na_proces;j++, k++){
				podmacierz[k]=macierz[j];
				printf("%d\t", podmacierz[k]);
			}
			printf("\n");
		}
	}
	else{
		printf("Macierz nie jest proporcjonalnie podzielna.");
	}
}

int main(int argc, char *argv[]){
	int *M, *m;						//macierz, podmacierz
	int w=1, h=8;					//w, h; //szerokosc, wysokosc
	int l_procesow=4, nr_procesu;	//l_procesow, nr_procesu;
	malloc_macierz(&M, w, h);
	malloc_podmacierz(&m, w, h, l_procesow);
	generuj_macierz(M, w, h);
	wypisz_macierz(M, w, h);
	printf("\n");
	dziel_macierz(M, m, w, h, l_procesow);
	return 0;
}
