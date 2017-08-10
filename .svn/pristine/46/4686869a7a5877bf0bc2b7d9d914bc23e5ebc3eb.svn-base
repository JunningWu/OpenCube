#ifndef _SERIALIZER_CC
#define _SERIALIZER_CC

extern "C" void *serializer_fun(void *args);

void *serializer_fun(void *args){

serializer_fifos();

	#define MASK      0x0001
	#define PRM_NO    57
	static const Word16 bitno[PRM_NO] =
	{
        7, 8, 9, 8, 6,                          /* LSP VQ          */
        9, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 5,  /* first subframe  */
        6, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 5,  /* second subframe */
        9, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 5,  /* third subframe  */
        6, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 5   /* fourth subframe */
    	};
	int 	parameter_number;
	bool	out_bit;
	Word16	parameter,bit_number;
	unsigned int par;

	BEGIN_PROF();

	parameter_number = 0;
	//	par = 0;		
	while(true) {
		#ifdef _PRINT_PARAMETERS						
		if(parameter_number==0) cout <<"SERIALIZER: Receiving Parameters."<<endl;
		#endif
		CH_MONITOR(parameter = prm_cn->read();)
		#ifdef _PRINT_PARAMETERS	 
		cout << parameter;
		#endif
		par = parameter;
		for(bit_number = bitno[parameter_number]-1; bit_number>=0;bit_number--) {  // first the MSB
			out_bit = (par & (1 << bit_number))!=0;
			CH_MONITOR(bit_stream_->write(out_bit);)
		}
		if(parameter_number<(PRM_NO-1))	parameter_number++;
		else {
			parameter_number = 0;
			#ifdef _PRINT_PARAMETERS
			cout<<endl;
			#endif
			}
	}
    
}//serializer_fun 
#endif
