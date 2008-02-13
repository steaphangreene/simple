# *************************************************************************
#  This file is part of the SimpleAudio Example Module by Steaphan Greene
#
#  Copyright 2005-2006 Steaphan Greene <stea@cs.binghamton.edu>
#
#  SimpleAudio is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  SimpleAudio is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with SimpleAudio (see the file named "COPYING");
#  if not, write to the the Free Software Foundation, Inc.,
#  59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
# *************************************************************************

all:	libsimpleaudio.a
#all:	test

win32:	libsimpleaudio.win32_a

#All of this is usually overridden by the main Makefile
CXX=	g++
FLAGS=	-g -Wall -DSDL_DEBUG=SDL_INIT_NOPARACHUTE `sdl-config --cflags` `zzip-config --cflags`
LIBS=	`sdl-config --libs` `zzip-config --libs` -lGL -lGLU
WCXX=	i586-mingw32msvc-g++
WFLAGS=	-s -O2 -Wall `/usr/i586-mingw32msvc/bin/i586-mingw32msvc-sdl-config --cflags`
WLIBS=	`/usr/i586-mingw32msvc/bin/i586-mingw32msvc-sdl-config --libs` -lSDL -lpng -ljpeg -lpng `/usr/i586-mingw32msvc/bin/i586-mingw32msvc-zzip-config --libs` -lopengl32 -lglu32

OBJS:=	simpleaudio.o
CXXFLAGS:=	$(FLAGS)
CXXLIBS:=	-lSDL_ttf -lSDL_image $(LIBS)

WOBJS:=	simpleaudio.win32_o
WCXXFLAGS:=	$(WFLAGS)
WCXXLIBS:=	-lSDL_ttf -lSDL_image $(WLIBS)

libsimpleaudio.a:	$(OBJS)
	rm -vf $@
	ar rcs $@ $(OBJS)

libsimpleaudio.win32_a:	$(WOBJS)
	rm -vf $@
	i586-mingw32msvc-ar rcs $@ $(WOBJS)

ChangeLog:	*.cpp *.h TODO COPYING Makefile .svn
	../scripts/svn2cl.sh | sed 's-  stea-  sgreene-g' > ChangeLog

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
	rm -f deps.mk *.o *.a *.win32_o *.win32_a test[0-9] *.exe ChangeLog

test:	test1

test1:	test1.o libsimpleaudio.a
	$(CXX) $(CXXFLAGS) -o test1 test1.o renderer.o libsimpleaudio.a $(CXXLIBS)
	
win32_test:	test1.exe
wintest:	win32_test

test1.exe:	test1.win32_o renderer.win32_o libsimpleaudio.win32_a
	$(WCXX) $(WCXXFLAGS) -o test1.exe test1.win32_o renderer.win32_o libsimpleaudio.win32_a $(WCXXLIBS)
	
TSTR:=	$(shell date -u +"%Y%m%d%H%M")
backup:	tar
tar:
	cd .. ; tar chvf simpleaudio/simpleaudio.$(TSTR).tar \
		simpleaudio/Makefile simpleaudio/TODO simpleaudio/COPYING \
		simpleaudio/*.cpp simpleaudio/*.h
#		simpleaudio/tests/*.cpp simpleaudio/tests/*.h \
#		simpleaudio/scripts/*.* simpleaudio/deps.mk \
	gzip -9 simpleaudio.$(TSTR).tar

include deps.mk
