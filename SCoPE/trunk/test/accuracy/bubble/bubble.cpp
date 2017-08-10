/*!
 * \file bubble.cpp
 * \brief Algoritmo de ordenacion de la burbuja
 * Fichero preparado para compilar con Skyeye
 */

#define N_ELEM 100

int uc_main(void) {
	int a[N_ELEM];
	int i,j,aux;
	for (i=0 ; i<N_ELEM ; i++) {
		a[i] = N_ELEM-i;
	}
	aux = 0;
	for (i=1 ; i<N_ELEM ; i++) {
		for (j=0 ; j<N_ELEM-i ; j++) {
			if (a[j] > a[j+1]) {
				aux = a[j];
				a[j] = a[j+1];
				a[j+1] = aux;
			}
		}
	}
	return 0;
}
