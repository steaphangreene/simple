all:	test


#PRODUCTION OPTIONS (STANDARD)
#CXX=	gcc
#CXXFLAGS=	-O2 -Wall `sdl-config --cflags`
#LIBS=	`sdl-config --libs` -lSDL_ttf -lGL -lGLU

#DEGUGGING OPTIONS
CXX=	gcc
CXXFLAGS=	-g -Wall -DSDL_DEBUG=SDL_INIT_NOPARACHUTE `sdl-config --cflags`
LIBS=	`sdl-config --libs` -lefence -lSDL_ttf -lGL -lGLU

#PRODUCTION OPTIONS (CROSS-COMPILED FOR WINDOWS)
#CXX=	i586-mingw32msvc-g++
#CXXFLAGS=	-O2 -Wall `/usr/i586-mingw32msvc/bin/i586-mingw32msvc-sdl-config --cflags`
#LIBS=	`/usr/i586-mingw32msvc/bin/i586-mingw32msvc-sdl-config --libs` -lopengl32 -lglu32

OBJS:=	simplegui.o sg_widget.o sg_alignment.o sg_table.o \
	sg_panel.o sg_textarea.o sg_button.o sg_passthrough.o

all: $(OBJS)

%.o:	%.cpp
	$(CXX) $(CXXFLAGS) -c $<

deps.mk:	*.cpp *.h
	$(CXX) $(CXXFLAGS) -MM *.cpp > deps.mk

clean:
	rm -f *.o test1 deps.mk

test:	test1

test1:	test1.o renderer.o audio.o $(OBJS)
	$(CXX) $(CXXFLAGS) -o test1 test1.o renderer.o audio.o $(OBJS) $(LIBS)

include deps.mk
