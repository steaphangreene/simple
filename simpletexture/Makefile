# *************************************************************************
#  This file is part of the SimpleTexture Example Module by Steaphan Greene
#
#  Copyright 2005-2006 Steaphan Greene <stea@cs.binghamton.edu>
#
#  SimpleTexture is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  SimpleTexture is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with SimpleTexture (see the file named "COPYING");
#  if not, write to the the Free Software Foundation, Inc.,
#  59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
# *************************************************************************

all:	libsimpletexture.a

win32:	libsimpletexture.win32_a

#PRODUCTION OPTIONS (STANDARD)
#CXX=	g++
#CXXFLAGS=	-s -O2 -Wall `sdl-config --cflags` `zzip-config --cflags`
#LIBS=	`sdl-config --libs` `zzip-config --libs` -lSDL_ttf -lSDL_image -lGL -lGLU

#PRODUCTION OPTIONS (WORKAROUND FOR MacOS-X)
#CXX=	g++
#CXXFLAGS=	-s -O2 -Wall `sdl-config --cflags` `zzip-config --cflags`
#LIBS=	`sdl-config --libs` `zzip-config --libs` -lSDL_ttf -lSDL_image -framework OpenGL

#PRODUCTION OPTIONS (WORKAROUND FOR CYGWIN)
#CXX=	g++
#CXXFLAGS=	-s -O2 -Wall `sdl-config --cflags` `zzip-config --cflags`
#LIBS=	`sdl-config --libs` `zzip-config --libs` -lSDL_ttf -lSDL_image -lopengl32 -lglu32

#DEGUGGING OPTIONS (NO EFENCE)
CXX=	g++
CXXFLAGS=	-g -Wall -DSDL_DEBUG=SDL_INIT_NOPARACHUTE `sdl-config --cflags` `zzip-config --cflags`
LIBS=	`sdl-config --libs` `zzip-config --libs` -lSDL_ttf -lSDL_image -lGL -lGLU

#DEGUGGING OPTIONS (WITH EFENCE)
#CXX=	g++
#CXXFLAGS=	-g -Wall -DSDL_DEBUG=SDL_INIT_NOPARACHUTE `sdl-config --cflags` `zzip-config --cflags`
#LIBS=	`sdl-config --libs` `zzip-config --libs` -lSDL_ttf -lSDL_image -lefence -lGL -lGLU

#PROFILING OPTIONS
#CXX=	g++
#CXXFLAGS=	-pg -g -Wall -DSDL_DEBUG=SDL_INIT_NOPARACHUTE `sdl-config --cflags` `zzip-config --cflags`
#LIBS=	`sdl-config --libs` `zzip-config --libs` -lSDL_ttf -lSDL_image -lGL -lGLU

OBJS:=	st.o stt.o stt_default.o stt_invisible.o \
	stt_button.o stt_buttonup.o stt_buttondown.o

#PRODUCTION OPTIONS (CROSS-COMPILED FOR WINDOWS)
WCXX=	i586-mingw32msvc-g++
WCXXFLAGS=	-s -O2 -Wall `/usr/i586-mingw32msvc/bin/i586-mingw32msvc-sdl-config --cflags` `/usr/i586-mingw32msvc/bin/i586-mingw32msvc-zzip-config --cflags`
WLIBS=	`/usr/i586-mingw32msvc/bin/i586-mingw32msvc-sdl-config --libs` -lSDL_ttf -lSDL_image -lSDL -lpng -ljpeg -lpng `/usr/i586-mingw32msvc/bin/i586-mingw32msvc-zzip-config --libs` -lopengl32 -lglu32

WOBJS:= st.win32_o stt.win32_o stt_default.win32_o stt_invisible.win32_o \
	stt_button.win32_o stt_buttonup.win32_o stt_buttondown.win32_o

libsimpletexture.a:	$(OBJS)
	ar rcs libsimpletexture.a $(OBJS)

libsimpletexture.win32_a:	$(WOBJS)
	i586-mingw32msvc-ar rcs libsimpletexture.win32_a $(WOBJS)

ChangeLog:	*.cpp *.h TODO COPYING Makefile .svn scripts/*
	./scripts/svn2cl.sh | sed 's-stea-sgreene-g' > ChangeLog

test:	test1

test1:	test1.o renderer.o libsimpletexture.a
	$(CXX) $(CXXFLAGS) -o test1 test1.o renderer.o libsimpletexture.a $(LIBS)

win32_test:	test1.exe
wintest:	win32_test

test1.exe:	test1.win32_o renderer.win32_o libsimpletexture.win32_a
	$(WCXX) $(WCXXFLAGS) -o test1.exe test1.win32_o renderer.win32_o libsimpletexture.win32_a $(WLIBS)

%.o:	%.cpp
	$(CXX) $(CXXFLAGS) -c $<

%.o:	tests/%.cpp
	$(CXX) $(CXXFLAGS) -c $<

%.win32_o:	%.cpp %.o
	$(WCXX) $(WCXXFLAGS) -c $< -o $@

%.win32_o:	tests/%.cpp %.o
	$(WCXX) $(WCXXFLAGS) -c $< -o $@

deps.mk:	*.cpp *.h
	$(CXX) $(CXXFLAGS) -MM *.cpp > deps.mk

clean:
	rm -f deps.mk *.o *.a *.win32_o *.win32_a *.exe

TSTR:=	$(shell date -u +"%Y%m%d%H%M")
backup:	tar
tar:
	cd .. ; tar chvf simpletexture/simpletexture.$(TSTR).tar \
		simpletexture/Makefile simpletexture/TODO simpletexture/COPYING \
		simpletexture/*.cpp simpletexture/*.h
#		simpletexture/tests/*.cpp simpletexture/tests/*.h \
#		simpletexture/scripts/*.* simpletexture/deps.mk \
	gzip -9 simpletexture.$(TSTR).tar

include deps.mk
