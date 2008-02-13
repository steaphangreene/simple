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

#All of this is usually overridden by the main Makefile
CXX=	g++
FLAGS=	-g -Wall -DSDL_DEBUG=SDL_INIT_NOPARACHUTE `sdl-config --cflags` `zzip-config --cflags`
LIBS=	`sdl-config --libs` `zzip-config --libs` -lGL -lGLU
WCXX=	i586-mingw32msvc-g++
WFLAGS=	-s -O2 -Wall `/usr/i586-mingw32msvc/bin/i586-mingw32msvc-sdl-config --cflags`
WLIBS=	`/usr/i586-mingw32msvc/bin/i586-mingw32msvc-sdl-config --libs` -lSDL -lpng -ljpeg -lpng `/usr/i586-mingw32msvc/bin/i586-mingw32msvc-zzip-config --libs` -lopengl32 -lglu32

OBJS:=	st.o stt.o stt_default.o stt_invisible.o \
	stt_button.o stt_buttonup.o stt_buttondown.o
CXXFLAGS:=	$(FLAGS)
CXXLIBS:=	-lSDL_ttf -lSDL_image $(LIBS)

WOBJS:= st.win32_o stt.win32_o stt_default.win32_o stt_invisible.win32_o \
	stt_button.win32_o stt_buttonup.win32_o stt_buttondown.win32_o
WCXXFLAGS:=	$(WFLAGS)
WCXXLIBS:=	-lSDL_ttf -lSDL_image $(WLIBS)

libsimpletexture.a:	$(OBJS)
	rm -vf $@
	ar rcs $@ $(OBJS)

libsimpletexture.win32_a:	$(WOBJS)
	rm -vf $@
	i586-mingw32msvc-ar rcs $@ $(WOBJS)

ChangeLog:	*.cpp *.h TODO COPYING Makefile .svn tests/*.cpp tests/*.h
	../scripts/svn2cl.sh | sed 's-  stea-  sgreene-g' > ChangeLog

test:	test1

test1:	test1.o renderer.o libsimpletexture.a
	$(CXX) $(CXXFLAGS) -o test1 test1.o renderer.o libsimpletexture.a $(CXXLIBS)

win32_test:	test1.exe
wintest:	win32_test

test1.exe:	test1.win32_o renderer.win32_o libsimpletexture.win32_a
	$(WCXX) $(WCXXFLAGS) -o test1.exe test1.win32_o renderer.win32_o libsimpletexture.win32_a $(WCXXLIBS)

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
	rm -f deps.mk *.o *.a *.win32_o *.win32_a test[0-9] *.exe ChangeLog

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
