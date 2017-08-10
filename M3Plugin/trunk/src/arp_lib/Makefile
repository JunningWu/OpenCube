INC_DIR = -I$(ARP_HOME)

all: 
	if ls lib; then ls; else mkdir lib; fi
	g++ -g -c src/*.cpp $(INC_DIR)
	g++ -shared -lrt *.o -o lib/libarp_scope.so
	rm ../../build/libarp_scope.so ../../build/libarp_rhel4_g++3.4.6.a -rf
	ln -s ../src/arp_lib/lib/libarp_scope.so ../../build/libarp_scope.so
	ln -s ../src/arp_lib/lib/libarp_rhel4_g++3.4.6.a ../../build/libarp_rhel4_g++3.4.6.a

clean:
	rm *.o lib/libarp_scope.so -rf

distclean:
	rm *.o lib/libarp_scope.so -rf
