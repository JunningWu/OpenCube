//
//      University of Cantabria. (SPAIN)
//     MICROELECTRONICS ENGINEERING GROUP, (GIM)
//
//     Author: Hector Posadas
//     Date :  January 2009
//
//
//  sc_fifo_SS_3.h
//
//  POSIX SW/SW implementation of a sc_fifo channel for IPC
//
//

#ifndef _SC_FIFO_SS
#define _SC_FIFO_SS

#include <string.h>
#include <mqueue.h>

template <class T>
class sc_fifo_SS {
public:
    // constructors
    sc_fifo_SS( const char* name_, int size_ = 16 ) {
	fifo_name=(char*)malloc(strlen(name_)+1);
	strcpy(fifo_name,name_);
	create_fifo( size_);
    }

    // blocking read
    void read( T& );
    T read();

    // blocking write
    void write( const T& );

    operator T ()
	{ return read(); }

    sc_fifo_SS<T>& operator = ( const T& a )
        { write( a ); return *this; }

    void create_fifo(int size_);

private:


	char *fifo_name;
	mqd_t queue;

};

//
// IMPLEMENTATIONS
//
template <class T>
inline void sc_fifo_SS<T>::create_fifo(int size_) {
	char complete_name[200]="/tmp/";
	mq_attr attribs;
	attribs.mq_flags=0;
	attribs.mq_maxmsg=size_;
	attribs.mq_msgsize=sizeof(T);
	strcat (complete_name, fifo_name);
	if((queue = mq_open(complete_name, O_RDWR | O_CREAT, 0x1ff, &attribs)) < 0){
		printf("Error when creating the queue %s\n",fifo_name);
		exit(-1);
	}
}

// blocking read

template <class T>
inline void sc_fifo_SS<T>::read(T& data) {
	if(mq_receive(queue,(char*)&data, sizeof(T),NULL) < 0){
		printf("Error when reading the queue %s\n",fifo_name);
		exit(-1);
	}

//printf("read fifo %s: data %d\n",fifo_name,data);
}

template <class T>
inline T sc_fifo_SS<T>::read() {
    T tmpdata;	
    read(tmpdata);
    return tmpdata;
}



template <class T>
inline void sc_fifo_SS<T>::write(const T& data) {
//printf("write fifo %s: data %d\n",fifo_name,data);
	if(mq_send(queue,(const char*)&data, sizeof(T),1) < 0){
		printf("Error when sending to the queue %s\n",fifo_name);
		exit(-1);
	}
}

#endif
