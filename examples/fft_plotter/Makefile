 FLAGS=-I/opt/vc/include/  -I/opt/vc/include/interface/vcos/pthreads/  -I/opt/vc/include/interface/vmcs_host/linux -I../../src/
 LIBS= -L/usr/local/lib/ -L/opt/vc/lib -lGLESv2 -lEGL -lbcm_host  -lSOIL -lasound  -lm 
 SRC = $(wildcard ../../src/*.c) fft_plotter.c 
 OBJECTS=$(SRC:.c=.o)
 EXECUTABLE=fft_plotter.bin
 CC=gcc

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(FLAGS) $(LIBS)  $(OBJECTS)  -o $@ -DGLPI -lfftw3

.c.o:
	$(CC) $(FLAGS) $(LIBS) -DGLPI -c  $<  -o  $@ 


clean:
	rm -f *.bin
	rm -f $(OBJECTS)
