# *************************************************************************
#  This file is part of the SimpleVideo Example Module by Steaphan Greene
#
#  Copyright 2005 Steaphan Greene <stea@cs.binghamton.edu>
#
#  SimpleVideo is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  SimpleVideo is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with SimpleVideo (see the file named "COPYING");
#  if not, write to the the Free Software Foundation, Inc.,
#  59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
# *************************************************************************

all:	libsimplevideo.a
#all:	test

win32:	libsimplevideo.win32_a

#PRODUCTION OPTIONS (STANDARD)
#CXX=	g++
#CXXFLAGS=	-s -O2 -Wall `sdl-config --cflags`
#LIBS=	`sdl-config --libs` -lSDL_image -lGLEW

#PRODUCTION OPTIONS (WORKAROUND FOR MacOS-X)
#CXX=	g++
#CXXFLAGS=	-s -O2 -Wall `sdl-config --cflags`
#LIBS=	`sdl-config --libs` -lSDL_image -framework OpenGL

#PRODUCTION OPTIONS (WORKAROUND FOR CYGWIN)
#CXX=	g++
#CXXFLAGS=	-s -O2 -Wall `sdl-config --cflags`
#LIBS=	`sdl-config --libs` -lSDL_image -L/usr/X11R6/bin -lopengl32 -lglu32

#DEGUGGING OPTIONS (NO EFENCE)
CXX=	g++
CXXFLAGS=	-g -Wall -DSDL_DEBUG=SDL_INIT_NOPARACHUTE `sdl-config --cflags`
LIBS=	`sdl-config --libs` -lSDL_image -lGLEW

#DEGUGGING OPTIONS (WITH EFENCE)
#CXX=	g++
#CXXFLAGS=	-g -Wall -DSDL_DEBUG=SDL_INIT_NOPARACHUTE `sdl-config --cflags`
#LIBS=	`sdl-config --libs` -lSDL_image -lefence -lGLEW

#PROFILING OPTIONS
#CXX=	g++
#CXXFLAGS=	-pg -g -Wall -DSDL_DEBUG=SDL_INIT_NOPARACHUTE `sdl-config --cflags`
#LIBS=	`sdl-config --libs` -lSDL_image -lGLEW

OBJS:=	sv.o sv_ortho.o sv_perspective.o

#PRODUCTION OPTIONS (CROSS-COMPILED FOR WINDOWS)
WCXX=	i586-mingw32msvc-g++
WCXXFLAGS=	-s -O2 -Wall `/usr/i586-mingw32msvc/bin/i586-mingw32msvc-sdl-config --cflags`
WLIBS=	`/usr/i586-mingw32msvc/bin/i586-mingw32msvc-sdl-config --libs` -lSDL_image -lSDL -lpng -ljpeg -lz -lpng -lopengl32 -lglu32

WOBJS:=	sv.win32_o sv_ortho.win32_o sv_perspective.win32_o

libsimplevideo.a:	$(OBJS)
	ar rcs libsimplevideo.a $(OBJS)

libsimplevideo.win32_a:	$(WOBJS)
	i586-mingw32msvc-ar rcs libsimplevideo.win32_a $(WOBJS)

ChangeLog:	*.cpp *.h TODO COPYING Makefile CVS scripts/*
	./scripts/cvs2cl.pl --hide-filenames --no-wrap --stdout | tr '\t' '@' | sed 's-^@-@* -g' | tr '@' '\t' | sed 's-stea-sgreene-g' > ChangeLog

%.o:	%.cpp
	$(CXX) $(CXXFLAGS) -c $<

%.o:	tests/%.cpp
	$(CXX) $(CXXFLAGS) -c $<

%.win32_o:	%.cpp %.o
	$(WCXX) $(WCXXFLAGS) -c $< -o $@

%.win32_o:	tests/%.cpp %.o
	$(WCXX) $(WCXXFLAGS) -c $< -o $@

deps.mk:	*.cpp *.h
#	$(CXX) $(CXXFLAGS) -MM *.cpp tests/*.cpp > deps.mk
	$(CXX) $(CXXFLAGS) -MM *.cpp > deps.mk

clean:
	rm -f deps.mk *.o *.a test1 test2 test3 *.win32_o *.win32_a *.exe

test:	test1

test1:	test1.o libsimplevideo.a
	$(CXX) $(CXXFLAGS) -o test1 test1.o renderer.o libsimplevideo.a $(LIBS)
	
win32_test:	test1.exe
wintest:	win32_test

test1.exe:	test1.win32_o renderer.win32_o libsimplevideo.win32_a
	$(WCXX) $(WCXXFLAGS) -o test1.exe test1.win32_o renderer.win32_o libsimplevideo.win32_a $(WLIBS)
	
TSTR:=	$(shell date -u +"%Y%m%d%H%M")
backup:	tar
tar:
	cd .. ; tar chvf simplevideo/simplevideo.$(TSTR).tar \
		simplevideo/Makefile simplevideo/TODO simplevideo/COPYING \
		simplevideo/*.cpp simplevideo/*.h
#		simplevideo/tests/*.cpp simplevideo/tests/*.h \
#		simplevideo/scripts/*.* simplevideo/deps.mk \
	gzip -9 simplevideo.$(TSTR).tar

include deps.mk