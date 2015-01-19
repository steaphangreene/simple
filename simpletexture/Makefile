# *************************************************************************
#  This file is part of the SimpleTexture Example Module by Steaphan Greene
#
#  Copyright 2005-2015 Steaphan Greene <steaphan@gmail.com>
#
#  SimpleTexture is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 3 of the License, or
#  (at your option) any later version.
#
#  SimpleTexture is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with SimpleTexture (see the file named "COPYING");
#  If not, see <http://www.gnu.org/licenses/>.
#
# *************************************************************************

ARCH = $(shell gcc -v 2>&1 | grep Target | cut -f2 -d" ")

all:	$(ARCH)/libsimpletexture.a

win32:	libsimpletexture.win32_a

#All of this is usually overridden by the main Makefile
CXX=	g++
FLAGS=	--std=c++11 -g -Wall -Werror -DSDL_DEBUG=SDL_INIT_NOPARACHUTE `pkg-config sdl2 gl --cflags` `pkg-config zziplib --cflags`
LIBS=	`pkg-config sdl2 gl --libs` `pkg-config zziplib --libs` -lGL -lGLU
WCXX=	i586-mingw32msvc-g++
WFLAGS=	-s -O2 -Wall -Werror `/opt/i586-mingw32msvc/bin/pkg-config sdl2 gl --cflags`
WLIBS=	`/opt/i586-mingw32msvc/bin/pkg-config sdl2 gl --libs` -lSDL -lpng -ljpeg -lpng `/opt/i586-mingw32msvc/bin/pkg-config zziplib --libs` -lopengl32 -lglu32

OBJS:=	$(ARCH)/st.o $(ARCH)/stt.o $(ARCH)/stt_default.o \
	$(ARCH)/stt_invisible.o $(ARCH)/stt_button.o \
	$(ARCH)/stt_buttonup.o $(ARCH)/stt_buttondown.o \
	$(ARCH)/stt_upbuttonup.o $(ARCH)/stt_upbuttondown.o \
	$(ARCH)/stt_downbuttonup.o $(ARCH)/stt_downbuttondown.o \
	$(ARCH)/stt_rightbuttonup.o $(ARCH)/stt_leftbuttondown.o \
	$(ARCH)/stt_leftbuttonup.o $(ARCH)/stt_rightbuttondown.o
CXXFLAGS:=	$(FLAGS)
CXXLIBS:=	-lSDL2_ttf -lSDL2_image $(LIBS)

WOBJS:= st.win32_o stt.win32_o stt_default.win32_o stt_invisible.win32_o \
	stt_button.win32_o stt_buttonup.win32_o stt_buttondown.win32_o
WCXXFLAGS:=	$(WFLAGS)
WCXXLIBS:=	-lSDL2_ttf -lSDL2_image $(WLIBS)

$(ARCH):
	mkdir -p $(ARCH)

$(ARCH)/libsimpletexture.a:	$(OBJS) | $(ARCH)
	rm -vf $@
	ar rcs $@ $(OBJS)

libsimpletexture.win32_a:	$(WOBJS)
	rm -vf $@
	i586-mingw32msvc-ar rcs $@ $(WOBJS)

ChangeLog:	*.cpp *.h TODO COPYING Makefile .svn tests/*.cpp tests/*.h
	../scripts/svn2cl.sh | sed 's-  stea-  sgreene-g' > ChangeLog

test:	test1

test1:	$(ARCH)/test1.o $(ARCH)/libsimpletexture.a ../simplevideo/$(ARCH)/libsimplevideo.a ../simplescene/$(ARCH)/libsimplescene.a ../simplemodel/$(ARCH)/libsimplemodel.a ../simplegui/$(ARCH)/libsimplegui.a
	$(CXX) $(CXXFLAGS) -o test1 $(ARCH)/test1.o $(ARCH)/libsimpletexture.a ../simplevideo/$(ARCH)/libsimplevideo.a ../simplescene/$(ARCH)/libsimplescene.a ../simplemodel/$(ARCH)/libsimplemodel.a ../simplegui/$(ARCH)/libsimplegui.a $(CXXLIBS)

win32_test:	test1.exe
wintest:	win32_test

test1.exe:	test1.win32_o libsimpletexture.win32_a
	$(WCXX) $(WCXXFLAGS) -o test1.exe test1.win32_o libsimpletexture.win32_a $(WCXXLIBS)

$(ARCH)/%.o:	%.cpp | $(ARCH)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(ARCH)/%.o:	tests/%.cpp | $(ARCH)
	$(CXX) $(CXXFLAGS) -I../simplevideo -c $< -o $@

%.win32_o:	%.cpp $(ARCH)/%.o
	$(WCXX) $(WCXXFLAGS) -c $< -o $@

%.win32_o:	tests/%.cpp $(ARCH)/%.o
	$(WCXX) $(WCXXFLAGS) -I../simplevideo -c $< -o $@

deps.$(ARCH).mk:	*.cpp *.h
#	$(CXX) $(CXXFLAGS) -MM *.cpp tests/*.cpp | sed "s|^\(.*\)\.o:|x86_64-linux-gnu/\1.o:|" > $@
	$(CXX) $(CXXFLAGS) -MM *.cpp | sed "s|^\(.*\)\.o:|x86_64-linux-gnu/\1.o:|" > $@

clean:
	rm -f deps.$(ARCH).mk $(ARCH)/*.o $(ARCH)/*.a *.win32_o *.win32_a test[0-9] *.exe ChangeLog
	mkdir -p $(ARCH)	# So rmdir doesn't fail
	rmdir $(ARCH)

TSTR:=	$(shell date -u +"%Y%m%d%H%M")
backup:	tar
tar:
	cd .. ; tar chvf simpletexture/simpletexture.$(TSTR).tar \
		simpletexture/Makefile simpletexture/TODO simpletexture/COPYING \
		simpletexture/*.cpp simpletexture/*.h
#		simpletexture/tests/*.cpp simpletexture/tests/*.h \
#		simpletexture/scripts/*.* simpletexture/deps.$(ARCH).mk \
	gzip -9 simpletexture.$(TSTR).tar

include deps.$(ARCH).mk
