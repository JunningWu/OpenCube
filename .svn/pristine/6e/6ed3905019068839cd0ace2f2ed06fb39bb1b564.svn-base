/*!
 * \file compare.c
 * \author Juan Castillo <castillo@teisa.unican.es>
 * \brief Program that compares both inputs and outputs of gsm coder-decoder
 */

#include <stdio.h>
#include <stdlib.h>

typedef short Word16;

int main(int argc, char *argv[]) {
	FILE *fd1, *fd2, *fd_in, *fd_out;
	int i;
	Word16 sample;
	char sample_value[10];
	char in_filename[] = "input_samples.dat";
	char out_filename[] = "output_samples.dat";
	
	if (argc != 3) {
		fprintf(stderr, "\n\tUsage:\n");
		fprintf(stderr, "\t\t $./compare file1 file2\n\n");
		exit(0);
	}
	
	if ((fd1 = fopen(argv[1], "rb")) == NULL) {
		fprintf (stderr, "Cannot open file '%s' !!\n", argv[1]);
		exit (0);
	}

	if ((fd2 = fopen(argv[2], "rb")) == 0) {
		fprintf (stderr, "Cannot open file '%s' !!\n", argv[2]);
		exit (0);
	}
	
	if ((fd_in = fopen(in_filename, "w")) == NULL) {
		fprintf (stderr, "Cannot open file '%s' !!\n", in_filename);
		exit (0);
	}

	if ((fd_out = fopen(out_filename, "w")) == 0) {
		fprintf (stderr, "Cannot open file '%s' !!\n", out_filename);
		exit (0);
	}

	//Read the binary file and write data to samples file:
	i = 0;
	while (fread(&sample, sizeof(Word16), 1, fd1) == 1) {
		//input samples:
		fprintf(fd_in, " %d ", i);
		sprintf(sample_value, "%d", sample);
		fprintf(fd_in, "%s ", sample_value);
		
		//output samples:
		fread(&sample, sizeof(Word16), 1, fd2);
		sprintf(sample_value, "%d", sample);
		fprintf(fd_in, "%s\n", sample_value);
		
		i++;
	} //End while
	
	i = 0;
	while (fread(&sample, sizeof(Word16), 1, fd2) == 1) {
		sprintf(sample_value, "%d", sample);
		fprintf(fd_out, " %d ", i);
		fprintf(fd_out, "%s\n", sample_value);
		i++;
	} //End while

	fclose(fd1);
	fclose(fd2);
	fclose(fd_in);
	fclose(fd_out);
	return 0;
}
