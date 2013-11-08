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

void thresh_macierz(int *macierz, int *podmacierz, int szerokosc, int wysokosc, int liczba_procesow, int THRESH){
	int i, j, k;
	if(((szerokosc*wysokosc) % liczba_procesow)==0){
		int l_pikseli_na_proces=(int)((szerokosc*wysokosc)/liczba_procesow);
		for(i=0;i<liczba_procesow;i++){
			for(j=i*l_pikseli_na_proces, k=0;j<l_pikseli_na_proces + i*l_pikseli_na_proces;j++, k++){
				podmacierz[k]=macierz[j];	//symulacja dzielenia macierzy
			}
			//symulacja rozsyłania podmacierzy do wszystkich procesów
			for(k=0;k<l_pikseli_na_proces; k++){
				if(podmacierz[k]>=THRESH)	//symulacja operacji Threshold
					podmacierz[k]=255;
				else
					podmacierz[k]=0;
			}
			//symulacja wysyłania podmacierzy do procesu 0
			for(j=i*l_pikseli_na_proces, k=0;j<l_pikseli_na_proces + i*l_pikseli_na_proces;j++, k++){
				macierz[j]=podmacierz[k];	//symulacja łączenia podmacierzy w macierz
			}
		}
	}
	else{
		printf("Macierz nie jest proporcjonalnie podzielna.");
	}
}
void zapisz_do_pliku(char *nazwa_pliku, int *macierz, int szerokosc, int wysokosc){
	int i, j;
	FILE *plik = fopen(nazwa_pliku, "wb");
	(void) fprintf(plik, "P6\n%d %d\n255\n", szerokosc, wysokosc);
	for(i=0;i<wysokosc;i++){
		for(j=0;j<szerokosc;j++){
			static unsigned char kolor[3];
			kolor[0]=macierz[j+i*szerokosc];
			kolor[1]=macierz[j+i*szerokosc];
			kolor[2]=macierz[j+i*szerokosc];
			(void) fwrite(kolor, 1, 3, plik);
		}
	}
}
int main(int argc, char *argv[]){
	int *M, *m;						//macierz, podmacierz
	int w=320, h=240;				//w, h; //szerokosc, wysokosc
	int THRESH=123;
	int l_procesow=4, nr_procesu;	//l_procesow, nr_procesu;
	malloc_macierz(&M, w, h);
	malloc_podmacierz(&m, w, h, l_procesow);
	generuj_macierz(M, w, h);
	zapisz_do_pliku("monochrome.ppm", M, w, h);
	//wypisz_macierz(M, w, h);
	printf("\n");
	thresh_macierz(M, m, w, h, l_procesow, THRESH);
	zapisz_do_pliku("thresh.ppm", M, w, h);
	//wypisz_macierz(M, w, h);
	return 0;
}
