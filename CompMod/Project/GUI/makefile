#
# Makefile for 'jaw097_FDTD'.
#
# Type 'make' or 'make jaw097_FDTD' to create the executable.
# Type 'make clean' to delete all temporaries.
#

CC = g++

# build target specs
CFLAGS = -O3 -O3 -O3 -O3 -DDEBUG -lfltk -lfltk_gl -lGLU -lGL -lXext -lX11 -lm
OUT_DIR = .
LIBS = 

# first target entry is the target invoked when typing 'make'
default: jaw097_FDTD

jaw097_FDTD: $(OUT_DIR)/jaw097_FDTD.cpp.o
	$(CC) $(CFLAGS) -o jaw097_FDTD $(OUT_DIR)/jaw097_FDTD.cpp.o $(LIBS)

$(OUT_DIR)/jaw097_FDTD.cpp.o: jaw097_FDTD.cpp jaw097_gui.h
	$(CC) $(CFLAGS) -o $(OUT_DIR)/jaw097_FDTD.cpp.o -c jaw097_FDTD.cpp

clean:
	rm -f jaw097_FDTD $(OUT_DIR)/*.o
