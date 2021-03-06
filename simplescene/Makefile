# *************************************************************************
#  This file is part of the SimpleScene Example Module by Steaphan Greene
#
#  Copyright 2005-2015 Steaphan Greene <steaphan@gmail.com>
#
#  SimpleScene is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 3 of the License, or
#  (at your option) any later version.
#
#  SimpleScene is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with SimpleScene (see the file named "COPYING");
#  If not, see <http://www.gnu.org/licenses/>.
#
# *************************************************************************

ARCH = $(shell gcc -v 2>&1 | grep Target | cut -f2 -d" ")

all:	$(ARCH)/libsimplescene.a
#all:	test

win32:	libsimplescene.win32_a

#All of this is usually overridden by the main Makefile
CXX=	g++
FLAGS=	--std=c++11 -g -Wall -Werror -DSDL_DEBUG=SDL_INIT_NOPARACHUTE `pkg-config sdl2 --cflags` `pkg-config zziplib --cflags`
LIBS=	`pkg-config sdl2 --libs` `pkg-config zziplib --libs` -lGL -lGLU
WCXX=	i586-mingw32msvc-g++
WFLAGS=	-s -O2 -Wall -Werror `/opt/i586-mingw32msvc/bin/pkg-config sdl2 --cflags`
WLIBS=	`/opt/i586-mingw32msvc/bin/pkg-config sdl2 --libs` -lSDL -lpng -ljpeg -lpng `/opt/i586-mingw32msvc/bin/pkg-config zziplib --libs` -lopengl32 -lglu32

OBJS:=	$(ARCH)/ss.o
CXXFLAGS:=	$(FLAGS) -I../simpletexture -I../simplemodel -I../simplevideo
CXXLIBS:=	-lSDL2_ttf -lSDL2_image $(LIBS) ../simpletexture/$(ARCH)/libsimpletexture.a

WOBJS:=	ss.win32_o
WCXXFLAGS:=	$(WFLAGS) -I../simpletexture -I../simplemodel -I../simplevideo
WCXXLIBS:=	-lSDL2_ttf -lSDL2_image $(WLIBS) ../simpletexture/$(ARCH)/libsimpletexture.a

$(ARCH):
	mkdir -p $(ARCH)

$(ARCH)/libsimplescene.a:	$(OBJS) | $(ARCH)
	rm -vf $@
	ar rcs $@ $(OBJS)

libsimplescene.win32_a:	$(WOBJS)
	rm -vf $@
	i586-mingw32msvc-ar rcs $@ $(WOBJS)

ChangeLog:	*.cpp *.h TODO COPYING Makefile .svn
	../scripts/svn2cl.sh | sed 's-  stea-  sgreene-g' > ChangeLog

$(ARCH)/%.o:	%.cpp | $(ARCH)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(ARCH)/%.o:	tests/%.cpp | $(ARCH)
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.win32_o:	%.cpp $(ARCH)/%.o
	$(WCXX) $(WCXXFLAGS) -c $< -o $@

%.win32_o:	tests/%.cpp $(ARCH)/%.o
	$(WCXX) $(WCXXFLAGS) -c $< -o $@

deps.$(ARCH).mk:	*.cpp *.h
#	$(CXX) $(CXXFLAGS) -MM *.cpp tests/*.cpp | sed "s|^\(.*\)\.o:|x86_64-linux-gnu/\1.o:|" > $@
	$(CXX) $(CXXFLAGS) -MM *.cpp | sed "s|^\(.*\)\.o:|x86_64-linux-gnu/\1.o:|" > $@

clean:
	rm -f deps.$(ARCH).mk $(ARCH)/*.o $(ARCH)/*.a *.win32_o *.win32_a test[0-9] *.exe ChangeLog
	mkdir -p $(ARCH)	# So rmdir doesn't fail
	rmdir $(ARCH)

test:	test1

test1:	$(ARCH)/test1.o $(ARCH)/libsimplescene.a
	$(CXX) $(CXXFLAGS) -o test1 $(ARCH)/test1.o $(ARCH)/libsimplescene.a $(CXXLIBS)
	
win32_test:	test1.exe
wintest:	win32_test

test1.exe:	test1.win32_o libsimplescene.win32_a
	$(WCXX) $(WCXXFLAGS) -o test1.exe test1.win32_o libsimplescene.win32_a $(WCXXLIBS)
	
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
