 FLAGS=-I/opt/vc/include/  -I/opt/vc/include/interface/vcos/pthreads/  -I/opt/vc/include/interface/vmcs_host/linux -I../../src/
 LIBS= -lGLESv2 -lEGL -lm -lbcm_host -L/opt/vc/lib -lSOIL -lwiringPi
 SRC = $(wildcard ../../src/*.c) vibration_capture.c liir.c
 OBJECTS=$(SRC:.c=.o)
 EXECUTABLE=vibration_capture.bin
 CC=gcc

all: $(SRC) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	g++ $(FLAGS) $(LIBS)  $(OBJECTS) $(OBJECTS_CPP)  -o $@ -DGLPIb -Wall

.c.o:
	$(CC) $(FLAGS) $(LIBS) -DGLPI -c  $<  -o  $@ 

clean:
	rm -f *.bin
	rm -f $(OBJECTS)

