#include <stdio.h> 
#include <windows.h> 
#include <complex> 
#include<complex.h> 
#include <mpi.h> 
#include <iostream> 
#define SIZE 800 
using namespace std;
int Max_Number_Iteration = 320;
int mandel(complex< long double > z0) {
	int i;
	complex < long double > z;
	z = z0;
	for (i = 1; i< Max_Number_Iteration; i++) {
		z = z*z + z0;
		if ((creal(z)*creal(z)) + (cimag(z)*cimag(z))>4.0) break;
	}

	return i;
}


int main(int argc, char *argv[]) {
	_int64 ctr1 = 0, ctr2 = 0, freq = 0;
	int acc = 0, i = 0;
	double xmin, xmax, ymin, ymax;
	int  j, rows, columns, rank, nproc;
	complex <double> z;
	int row[SIZE], hdr;
	unsigned char line[3 * SIZE];
	FILE *img;

	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &nproc);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	img = fopen("mandel.pam", "w");
	MPI_Barrier(MPI_COMM_WORLD);

	if (rank == 0) hdr = fprintf(img, "P6\n%d %d 255\n", SIZE, SIZE);

	MPI_Bcast(&hdr, 1, MPI_INT, 0, MPI_COMM_WORLD);

	xmin = -2; xmax = -1;
	ymin = 0; ymax = 1;
	if (QueryPerformanceCounter((LARGE_INTEGER *)&ctr1) != 0)
	{
		for (i = (rank*SIZE) / nproc; i < ((rank + 1)*SIZE) / nproc; i++) {
			for (j = 0; j < SIZE; j++) {
				z = xmin + j*((xmax - xmin) / SIZE) + (ymax - i*((ymax - ymin) / SIZE))*i;
				row[j] = mandel(z);
			}

			for (j = 0; j < SIZE; j++) {
				if (row[j] <= 63) {
					line[3 * j] = 255;
					line[3 * j + 1] = line[3 * j + 2] = 255 - 4 * row[j];
				}
				else {
					line[3 * j] = 255;
					line[3 * j + 1] = row[j] - 63;
					line[3 * j + 2] = 0;
				}
				if (row[j] == 320) line[3 * j] = line[3 * j + 1] = line[3 * j + 2] = 255;
			}

			fseek(img, hdr + 3 * SIZE*i, SEEK_SET);
			fwrite(line, 1, 3 * SIZE, img);
		}
		QueryPerformanceCounter((LARGE_INTEGER *)&ctr2);
		cout << "start" << ctr1 << endl;
		cout << "end" << ctr2 << endl;
		QueryPerformanceFrequency((LARGE_INTEGER *)&freq);
		cout << "ctr1 - ctr2 = " << ctr2 - ctr1 << endl;
		cout << "QueryPerformanceCounter minimum resolution : 1/ " << freq << "seconds" << endl;
		cout << "Function takes time: " << ((ctr2 - ctr1) * 1.0 / freq) * 1000000 << " Microseconds." << endl;
	}

	MPI_Finalize();
	system("PAUSE");
	return 0;
}