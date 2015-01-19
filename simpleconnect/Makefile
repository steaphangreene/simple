# *************************************************************************
#  This file is part of the SimpleConnect Example Module by Steaphan Greene
#
#  Copyright 2005-2015 Steaphan Greene <steaphan@gmail.com>
#
#  SimpleConnect is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 3 of the License, or
#  (at your option) any later version.
#
#  SimpleConnect is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with SimpleConnect (see the file named "COPYING");
#  If not, see <http://www.gnu.org/licenses/>.
#
# *************************************************************************

ARCH = $(shell gcc -v 2>&1 | grep Target | cut -f2 -d" ")

all:	$(ARCH)/libsimpleconnect.a
#all:	test

win32:	libsimpleconnect.win32_a

#All of this is usually overridden by the main Makefile
CXX=	g++
FLAGS=	--std=c++11 -g -Wall -Werror -DSDL_DEBUG=SDL_INIT_NOPARACHUTE `pkg-config sdl2 --cflags` `pkg-config zziplib --cflags`
LIBS=	`pkg-config sdl2 --libs` `pkg-config zziplib --libs` -lGL -lGLU
WCXX=	i586-mingw32msvc-g++
WFLAGS=	-s -O2 -Wall -Werror `/opt/i586-mingw32msvc/bin/pkg-config sdl2 --cflags`
WLIBS=	`/opt/i586-mingw32msvc/bin/pkg-config sdl2 --libs` -lSDL -lpng -ljpeg -lpng `/opt/i586-mingw32msvc/bin/pkg-config zziplib --libs` -lopengl32 -lglu32

OBJS:=	$(ARCH)/sc.o
CXXFLAGS:=	$(FLAGS) -I../simplegui -I../simpletexture -I../simpleutils -I../simplevideo -I../simplescene -I../simplemodel
CXXLIBS:=	-lSDL2_net -lSDL2_ttf -lSDL2_image $(LIBS) ../simplegui/$(ARCH)/libsimplegui.a ../simpletexture/$(ARCH)/libsimpletexture.a  ../simpleutils/$(ARCH)/libsimpleutils.a ../simplevideo/$(ARCH)/libsimplevideo.a ../simplescene/$(ARCH)/libsimplescene.a ../simplemodel/$(ARCH)/libsimplemodel.a

WOBJS:=	sc.win32_o
WCXXFLAGS:=	$(WFLAGS) -I../simpletexture -I../simplegui -I../simpleutils -I../simplevideo -I../simplescene -I../simplemodel
WCXXLIBS:=	-lSDL2_net -lSDL2_ttf -lSDL2_image $(WLIBS) ../simplegui/$(ARCH)/libsimplegui.a ../simpletexture/$(ARCH)/libsimpletexture.a ../simpleutils/$(ARCH)/libsimpleutils.a ../simplevideo/$(ARCH)/libsimplevideo.a ../simplescene/$(ARCH)/libsimplescene.a ../simplemodel/$(ARCH)/libsimplemodel.a

$(ARCH):
	mkdir -p $(ARCH)

$(ARCH)/libsimpleconnect.a:	$(OBJS) | $(ARCH)
	rm -vf $@
	ar rcs $@ $(OBJS)

libsimpleconnect.win32_a:	$(WOBJS)
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
	$(CXX) $(CXXFLAGS) -MM *.cpp tests/*.cpp | sed "s|^\(.*\)\.o:|x86_64-linux-gnu/\1.o:|" > $@

clean:
	rm -f deps.$(ARCH).mk $(ARCH)/*.o $(ARCH)/*.a *.win32_o *.win32_a test[0-9] *.exe ChangeLog
	mkdir -p $(ARCH)	# So rmdir doesn't fail
	rmdir $(ARCH)

test:	test1

test1:	$(ARCH)/test1.o $(ARCH)/libsimpleconnect.a $(ARCH)/audio.o
	$(CXX) $(CXXFLAGS) -o test1 $(ARCH)/test1.o $(ARCH)/audio.o $(ARCH)/libsimpleconnect.a $(CXXLIBS)

win32_test:	test1.exe
wintest:	win32_test

test1.exe:	test1.win32_o audio.win32_o libsimpleconnect.win32_a
	$(WCXX) $(WCXXFLAGS) -o test1.exe test1.win32_o audio.win32_o libsimpleconnect.win32_a $(WCXXLIBS)
	
TSTR:=	$(shell date -u +"%Y%m%d%H%M")
backup:	tar
tar:
	cd .. ; tar chvf simpleconnect/simpleconnect.$(TSTR).tar \
		simpleconnect/Makefile simpleconnect/TODO simpleconnect/COPYING \
		simpleconnect/*.cpp simpleconnect/*.h
		simpleconnect/tests/*.cpp simpleconnect/tests/*.h \
#		simpleconnect/scripts/*.* simpleconnect/deps.$(ARCH).mk \
	gzip -9 simpleconnect.$(TSTR).tar

include deps.$(ARCH).mk
