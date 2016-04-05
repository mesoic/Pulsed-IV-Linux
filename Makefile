LIB_SOURCE = ./src/libs/
GUI_SOURCE = ./src/gui/
CC         = gcc

CFLAGS   = -c -I../inc/
OPTFLAGS  =`pkg-config --cflags gtk+-3.0 cairo`    
LDFLAGS  = `pkg-config --libs gtk+-3.0 cairo` /usr/local/lib/libgpib.so.0
PROGRAM  = PulseMaster

all: _bin
	cd build; \
	mv $(PROGRAM) ../ \

_bin: _libs
	cd $(GUI_SOURCE); \
	$(CC) $(CFLAGS) $(OPTFLAGS) pulse_gui.c; \
	mv *.o ../../build ; \
	cd ../../build ; \
	$(CC) -o $(PROGRAM) *.o $(LDFLAGS) ;  

_libs: 
	mkdir build ; \
	cd $(LIB_SOURCE); \
	$(CC) $(CFLAGS) *.c; \
	mv *.o ../../build ;

clean:
	rm -rf build; \
	rm $(PROGRAM); \
