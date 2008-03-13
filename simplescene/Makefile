# *************************************************************************
#  This file is part of the SimpleScene Example Module by Steaphan Greene
#
#  Copyright 2005-2008 Steaphan Greene <stea@cs.binghamton.edu>
#
#  SimpleScene is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  SimpleScene is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with SimpleScene (see the file named "COPYING");
#  if not, write to the the Free Software Foundation, Inc.,
#  59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
# *************************************************************************

ARCH = $(shell gcc -v 2>&1 | grep Target | cut -f2 -d" ")

all:	$(ARCH)/libsimplescene.a
#all:	test

win32:	libsimplescene.win32_a

#All of this is usually overridden by the main Makefile
CXX=	g++
FLAGS=	-g -Wall -DSDL_DEBUG=SDL_INIT_NOPARACHUTE `sdl-config --cflags` `zzip-config --cflags`
LIBS=	`sdl-config --libs` `zzip-config --libs` -lGL -lGLU
WCXX=	i586-mingw32msvc-g++
WFLAGS=	-s -O2 -Wall `/usr/i586-mingw32msvc/bin/i586-mingw32msvc-sdl-config --cflags`
WLIBS=	`/usr/i586-mingw32msvc/bin/i586-mingw32msvc-sdl-config --libs` -lSDL -lpng -ljpeg -lpng `/usr/i586-mingw32msvc/bin/i586-mingw32msvc-zzip-config --libs` -lopengl32 -lglu32

OBJS:=	$(ARCH)/ss.o
CXXFLAGS:=	$(FLAGS) -I../simpletexture
CXXLIBS:=	-lSDL_ttf -lSDL_image $(LIBS) ../simpletexture/$(ARCH)/libsimpletexture.a

WOBJS:=	ss.win32_o
WCXXFLAGS:=	$(WFLAGS) -I../simpletexture
WCXXLIBS:=	-lSDL_ttf -lSDL_image $(WLIBS) ../simpletexture/$(ARCH)/libsimpletexture.a

$(ARCH)/libsimplescene.a:	$(OBJS)
	mkdir -p $(ARCH)
	rm -vf $@
	ar rcs $@ $(OBJS)

libsimplescene.win32_a:	$(WOBJS)
	rm -vf $@
	i586-mingw32msvc-ar rcs $@ $(WOBJS)

ChangeLog:	*.cpp *.h TODO COPYING Makefile .svn
	../scripts/svn2cl.sh | sed 's-  stea-  sgreene-g' > ChangeLog

$(ARCH)/%.o:	%.cpp
	mkdir -p $(ARCH)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(ARCH)/%.o:	tests/%.cpp
	mkdir -p $(ARCH)
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.win32_o:	%.cpp $(ARCH)/%.o
	$(WCXX) $(WCXXFLAGS) -c $< -o $@

%.win32_o:	tests/%.cpp $(ARCH)/%.o
	$(WCXX) $(WCXXFLAGS) -c $< -o $@

deps.$(ARCH).mk:	*.cpp *.h
#	$(CXX) $(CXXFLAGS) -MM *.cpp tests/*.cpp | sed "s/\.o:/.$(ARCH).o:/" > $@
	$(CXX) $(CXXFLAGS) -MM *.cpp | sed "s/\.o:/.$(ARCH).o:/" > $@

clean:
	rm -f deps.$(ARCH).mk $(ARCH)/*.o $(ARCH)/*.a *.win32_o *.win32_a test[0-9] *.exe ChangeLog
	mkdir -p $(ARCH)
	rmdir $(ARCH)

test:	test1

test1:	test1.o $(ARCH)/libsimplescene.a
	$(CXX) $(CXXFLAGS) -o test1 test1.o renderer.o $(ARCH)/libsimplescene.a $(CXXLIBS)
	
win32_test:	test1.exe
wintest:	win32_test

test1.exe:	test1.win32_o renderer.win32_o libsimplescene.win32_a
	$(WCXX) $(WCXXFLAGS) -o test1.exe test1.win32_o renderer.win32_o libsimplescene.win32_a $(WCXXLIBS)
	
TSTR:=	$(shell date -u +"%Y%m%d%H%M")
backup:	tar
tar:
	cd .. ; tar chvf simplescene/simplescene.$(TSTR).tar \
		simplescene/Makefile simplescene/TODO simplescene/COPYING \
		simplescene/*.cpp simplescene/*.h
#		simplescene/tests/*.cpp simplescene/tests/*.h \
#		simplescene/scripts/*.* simplescene/deps.$(ARCH).mk \
	gzip -9 simplescene.$(TSTR).tar

include deps.$(ARCH).mk
