# *************************************************************************
#  This file is part of the SimpleGUI Example Module by Steaphan Greene
#
#  Copyright 2005 Steaphan Greene <stea@cs.binghamton.edu>
#
#  SimpleGUI is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  SimpleGUI is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with SimpleGUI (see the file named "COPYING");
#  if not, write to the the Free Software Foundation, Inc.,
#  59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
# *************************************************************************

all:	$(OBJS)


#PRODUCTION OPTIONS (STANDARD)
CXX=	gcc
CXXFLAGS=	-O2 -Wall `sdl-config --cflags`
LIBS=	`sdl-config --libs` -lSDL_ttf -lGL -lGLU

#PRODUCTION OPTIONS (WORKAROUND FOR CYGWIN)
#CXX=	gcc
#CXXFLAGS=	-O2 -Wall `sdl-config --cflags`
#LIBS=	`sdl-config --libs` -lSDL_ttf -L/usr/X11R6/bin -lopengl32 -lglu32

#DEGUGGING OPTIONS
#CXX=	gcc
#CXXFLAGS=	-g -Wall -DSDL_DEBUG=SDL_INIT_NOPARACHUTE `sdl-config --cflags`
#LIBS=	`sdl-config --libs` -lefence -lSDL_ttf -lGL -lGLU

#PRODUCTION OPTIONS (CROSS-COMPILED FOR WINDOWS)
#CXX=	i586-mingw32msvc-g++
#CXXFLAGS=	-O2 -Wall `/usr/i586-mingw32msvc/bin/i586-mingw32msvc-sdl-config --cflags`
#LIBS=	`/usr/i586-mingw32msvc/bin/i586-mingw32msvc-sdl-config --libs` -lopengl32 -lglu32

OBJS:=	simplegui.o sg_widget.o sg_alignment.o sg_table.o \
	sg_panel.o sg_textarea.o sg_button.o sg_progressbar.o \
	sg_compound.o sg_filebrowser.o sg_multitab.o sg_listbox.o \
	sg_radiobuttons.o sg_combobox.o sg_pulldown.o sg_sliderbar.o \
	sg_scrollingarea.o \
	sg_passthrough.o sg_translabel.o

all: $(OBJS)

%.o:	%.cpp
	$(CXX) $(CXXFLAGS) -c $<

%.o:	tests/%.cpp
	$(CXX) $(CXXFLAGS) -c $<

deps.mk:	*.cpp *.h
	$(CXX) $(CXXFLAGS) -MM *.cpp tests/*.cpp > deps.mk

clean:
	rm -f deps.mk *.o test1

test:	test1

test1:	test1.o renderer.o audio.o $(OBJS)
	$(CXX) $(CXXFLAGS) -o test1 test1.o renderer.o audio.o $(OBJS) $(LIBS)

TSTR:=	$(shell date -u +"%Y%m%d%H%M")
backup:	tar
tar:
	cd .. ; tar chvf simplegui/simplegui.$(TSTR).tar \
		simplegui/Makefile simplegui/TODO simplegui/COPYING \
		simplegui/*.cpp simplegui/*.h \
		simplegui/tests/*.cpp simplegui/tests/*.h \
		simplegui/scripts/*.* simplegui/deps.mk
	gzip -9 simplegui.$(TSTR).tar

include deps.mk
