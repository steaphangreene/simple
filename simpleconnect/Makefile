# *************************************************************************
#  This file is part of the SimpleConnect Example Module by Steaphan Greene
#
#  Copyright 2005-2008 Steaphan Greene <stea@cs.binghamton.edu>
#
#  SimpleConnect is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  SimpleConnect is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with SimpleConnect (see the file named "COPYING");
#  if not, write to the the Free Software Foundation, Inc.,
#  59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
# *************************************************************************

all:	libsimpleconnect.a
#all:	test

win32:	libsimpleconnect.win32_a

#All of this is usually overridden by the main Makefile
CXX=	g++
FLAGS=	-g -Wall -DSDL_DEBUG=SDL_INIT_NOPARACHUTE `sdl-config --cflags` `zzip-config --cflags`
LIBS=	`sdl-config --libs` `zzip-config --libs` -lGL -lGLU
WCXX=	i586-mingw32msvc-g++
WFLAGS=	-s -O2 -Wall `/usr/i586-mingw32msvc/bin/i586-mingw32msvc-sdl-config --cflags`
WLIBS=	`/usr/i586-mingw32msvc/bin/i586-mingw32msvc-sdl-config --libs` -lSDL -lpng -ljpeg -lpng `/usr/i586-mingw32msvc/bin/i586-mingw32msvc-zzip-config --libs` -lopengl32 -lglu32

OBJS:=	sc.o
CXXFLAGS:=	$(FLAGS) -I../simplegui -I../simpletexture
CXXLIBS:=	-lSDL_net -lSDL_ttf -lSDL_image $(LIBS) ../simplegui/libsimplegui.a ../simpletexture/libsimpletexture.a

WOBJS:=	sc.win32_o
WCXXFLAGS:=	$(WFLAGS) -I../simpletexture -I../simplegui
WCXXLIBS:=	-lSDL_net -lSDL_ttf -lSDL_image $(WLIBS) ../simplegui/libsimplegui.a ../simpletexture/libsimpletexture.a

libsimpleconnect.a:	$(OBJS)
	rm -vf $@
	ar rcs $@ $(OBJS)

libsimpleconnect.win32_a:	$(WOBJS)
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

test1:	test1.o libsimpleconnect.a
	$(CXX) $(CXXFLAGS) -o test1 test1.o renderer.o libsimpleconnect.a $(CXXLIBS)
	
win32_test:	test1.exe
wintest:	win32_test

test1.exe:	test1.win32_o renderer.win32_o libsimpleconnect.win32_a
	$(WCXX) $(WCXXFLAGS) -o test1.exe test1.win32_o renderer.win32_o libsimpleconnect.win32_a $(WCXXLIBS)
	
TSTR:=	$(shell date -u +"%Y%m%d%H%M")
backup:	tar
tar:
	cd .. ; tar chvf simpleconnect/simpleconnect.$(TSTR).tar \
		simpleconnect/Makefile simpleconnect/TODO simpleconnect/COPYING \
		simpleconnect/*.cpp simpleconnect/*.h
#		simpleconnect/tests/*.cpp simpleconnect/tests/*.h \
#		simpleconnect/scripts/*.* simpleconnect/deps.mk \
	gzip -9 simpleconnect.$(TSTR).tar

include deps.mk
