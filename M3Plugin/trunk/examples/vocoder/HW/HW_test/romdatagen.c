#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../c_base/common/typedef.h"

//#define _UNIX_VERSION

//
// Valid file fo dos-based platforms (little endian)
// for TEST0.inp files for little endian
//

int main (int argc, char *argv[]) {
	
    FILE *f_in, *f_out, *h_out;
	Word16 framenum,n_frames,frame_size,width,i;
	
#ifdef _UNIX_VERSION	
	Word16 temp1,temp2;
#endif
	
	Word16 frame[250];
	Word16 file_sample;
	
	char romname[100];
	char hname[100];
	
    if (argc != 6)
	{
        fprintf (stderr,
                 "   Usage:\n\n romdatagen input_file output_file n_frames frame_size width\n");
        fprintf (stderr, "\n");
        exit (1);
    }
    if ((f_in = fopen (argv[1], "rb")) == NULL)
    {
        fprintf (stderr, "Input file '%s' does not exist !!\n", argv[1]);
        exit (0);
    }
    else
        fprintf (stderr, "Input file:   %s\n", argv[1]);

    if ((f_out = fopen (argv[2], "wb")) == NULL)
    {
        fprintf (stderr, "Cannot open file '%s' !!\n", argv[2]);
        exit (0);
    }
    else
        fprintf (stderr, "output file:   %s\n", argv[2]);
	
	for (i=0;i<100;i++)
	{
		if (argv[2][i]=='.')
			break;
		else
			romname[i]=argv[2][i];
			hname[i]=argv[2][i];
	}
	strcat(hname,"_content.h");
    if ((h_out = fopen (hname, "wb")) == NULL)
    {
        fprintf (stderr, "Cannot open file '%s' !!\n", hname);
        exit (0);
    }
    else
        fprintf (stderr, "h file:   %s\n", hname);
	
 
 	n_frames=atoi(argv[3]);
 	frame_size=atoi(argv[4]);
 	width=atoi(argv[5]);
 
 	fprintf (f_out,"Component_Name=%s;\n",romname);
 	fprintf (f_out,"Width=%d;\n",width);
 	fprintf (f_out,"Depth=%d;\n",n_frames*frame_size);
 	fprintf (f_out,"Memory_Initialization_Radix=16;\n");
 	fprintf (f_out,"Memory_Initialization_Vector=\n");
	
 	//fprintf (h_out,"const sc_uint<ROM_NS_DATA_SIZE> %s_array[ROM_NS_DEPTH] = {\n",romname);

	for(framenum=0;framenum<n_frames;framenum++) {
		if(fread (frame, sizeof (Word16), frame_size, f_in) == frame_size) {
			for(i=0;i<frame_size;i++) {
				// For Unix Version over TEST0 for little endian
				
#ifdef _UNIX_VERSION				
				temp1=frame[i];
				temp2=frame[i];
				frame[i]= ((temp1 << 8)&0xff00) | ((temp2 >> 8)&0x00ff);
#endif
				file_sample = frame[i];
				
				if(width>3)
					frame[i]=(frame[i]>>3)&0x1fff;

				// .coe generation
				if (width<=4)
					fprintf (f_out,"%01hX",frame[i]);
				else if (width<=8)
					fprintf (f_out,"%02hX",frame[i]);
				else if (width<=12)
					fprintf (f_out,"%03hX",frame[i]);
				else if (width<=16)
					fprintf (f_out,"%04hX",frame[i]);
								
				if ((framenum==n_frames-1)&&(i==frame_size-1))
					fprintf (f_out,";\n");
				else
					fprintf (f_out,",\n");
				
				// .h generation for SystemC ROM model
				fprintf (h_out,"%s_array[%d]=%hd;\n",romname,(framenum*frame_size)+i,file_sample);
			}
		}
		else {
			fprintf (stderr, "Warning: n_frames bigger than those in input_file\n");
			fprintf (stderr, "Output file could have erroneus format\n");
			fprintf (stderr, "Readed frames: %d\n",framenum);
			return 0;
		}
	}
		
	return 0;
}
