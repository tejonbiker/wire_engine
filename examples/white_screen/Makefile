 FLAGS=-I/opt/vc/include/  -I/opt/vc/include/interface/vcos/pthreads/  -I/opt/vc/include/interface/vmcs_host/linux -I../../src/
 LIBS= -lGLESv2 -lEGL -lm -lbcm_host -L/opt/vc/lib
 SRC= ../../src/glpi.c white_screen.c
 OBJECTS=$(SRC:.c=.o)
 EXECUTABLE=cube_light_point.bin
 CC=gcc

all: $(SRC) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	#gcc $(FLAGS)  -o $@ $(SRC) $< $(LIBS) -DGLPI  -std=gnu99
	$(CC) $(FLAGS) $(LIBS)  $(OBJECTS)  -o $@ -DGLPI

.c.o:
	$(CC) $(FLAGS) $(LIBS) -DGLPI -c  $<  -o  $@ 

clean:
	rm -f *.bin
	rm -f $(OBJECTS)

