#
# Makefile for 'epub'.
#
# Type 'make' or 'make epub' to create the executable.
# Type 'make clean' to delete all temporaries.
#

CC = g++

# build target specs
CFLAGS = -O3 -O3 -DDEBUG -lfltk -lfltk_gl -lGLU -lGL -lXext -lX11 -lm
OUT_DIR = release_build
LIBS = 

# first target entry is the target invoked when typing 'make'
default: epub

epub: $(OUT_DIR)/epub2.cpp.o $(OUT_DIR)/Meta.cxx.o $(OUT_DIR)/about.cxx.o $(OUT_DIR)/epubfunctions.cxx.o $(OUT_DIR)/epubmk.cxx.o $(OUT_DIR)/images.cxx.o
	$(CC) $(CFLAGS) -o epub $(OUT_DIR)/epub2.cpp.o $(OUT_DIR)/Meta.cxx.o $(OUT_DIR)/about.cxx.o $(OUT_DIR)/epubfunctions.cxx.o $(OUT_DIR)/epubmk.cxx.o $(OUT_DIR)/images.cxx.o $(LIBS)

$(OUT_DIR)/epub2.cpp.o: epub2.cpp epubfunctions.h epubmk.h Meta.h about.h \
 images.cxx
	$(CC) $(CFLAGS) -o $(OUT_DIR)/epub2.cpp.o -c epub2.cpp

$(OUT_DIR)/Meta.cxx.o: Meta.cxx Meta.h
	$(CC) $(CFLAGS) -o $(OUT_DIR)/Meta.cxx.o -c Meta.cxx

$(OUT_DIR)/about.cxx.o: about.cxx about.h images.cxx
	$(CC) $(CFLAGS) -o $(OUT_DIR)/about.cxx.o -c about.cxx

$(OUT_DIR)/epubfunctions.cxx.o: epubfunctions.cxx epubfunctions.h
	$(CC) $(CFLAGS) -o $(OUT_DIR)/epubfunctions.cxx.o -c epubfunctions.cxx

$(OUT_DIR)/epubmk.cxx.o: epubmk.cxx epubmk.h Meta.h epubfunctions.h
	$(CC) $(CFLAGS) -o $(OUT_DIR)/epubmk.cxx.o -c epubmk.cxx

$(OUT_DIR)/images.cxx.o: images.cxx
	$(CC) $(CFLAGS) -o $(OUT_DIR)/images.cxx.o -c images.cxx

clean:
	rm -f epub $(OUT_DIR)/*.o

profile:
	gprof epub > profile.txt; ./gprof2dot.py profile.txt | dot -Tpng -o profile.png
