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
#all:	test

#PRODUCTION OPTIONS (STANDARD)
#CXX=	gcc
#CXXFLAGS=	-s -O2 -Wall `sdl-config --cflags`
#LIBS=	`sdl-config --libs` -lSDL_ttf -lGL -lGLU

#PRODUCTION OPTIONS (WORKAROUND FOR CYGWIN)
#CXX=	gcc
#CXXFLAGS=	-s -O2 -Wall `sdl-config --cflags`
#LIBS=	`sdl-config --libs` -lSDL_ttf -L/usr/X11R6/bin -lopengl32 -lglu32

#DEGUGGING OPTIONS (NO EFENCE)
CXX=	gcc
CXXFLAGS=	-g -Wall -DSDL_DEBUG=SDL_INIT_NOPARACHUTE `sdl-config --cflags`
LIBS=	`sdl-config --libs` -lSDL_ttf -lGL -lGLU

#DEGUGGING OPTIONS (WITH EFENCE)
#CXX=	gcc
#CXXFLAGS=	-g -Wall -DSDL_DEBUG=SDL_INIT_NOPARACHUTE `sdl-config --cflags`
#LIBS=	`sdl-config --libs` -lefence -lSDL_ttf -lGL -lGLU

#PROFILING OPTIONS
#CXX=	gcc
#CXXFLAGS=	-pg -g -Wall -DSDL_DEBUG=SDL_INIT_NOPARACHUTE `sdl-config --cflags`
#LIBS=	`sdl-config --libs` -lSDL_ttf -lGL -lGLU

OBJS:=	sg.o sg_widget.o sg_texture.o \
	sg_alignment.o sg_table.o sg_scrollable.o \
	sg_panel.o sg_animatedpanel.o \
	sg_textarea.o sg_progressbar.o sg_editable.o \
	sg_button.o sg_stickybutton.o sg_menu.o sg_dragable.o \
	sg_compound.o sg_filebrowser.o sg_multitab.o sg_listbox.o \
	sg_radiobuttons.o sg_combobox.o sg_pulldown.o sg_sliderbar.o \
	sg_scrollingarea.o sg_dndboxes.o sg_tabs.o \
	sg_passthrough.o sg_translabel.o

#PRODUCTION OPTIONS (CROSS-COMPILED FOR WINDOWS)
WCXX=	i586-mingw32msvc-g++
WCXXFLAGS=	-s -O2 -Wall `/usr/i586-mingw32msvc/bin/i586-mingw32msvc-sdl-config --cflags`
WLIBS=	`/usr/i586-mingw32msvc/bin/i586-mingw32msvc-sdl-config --libs` -lSDL_ttf -lopengl32 -lglu32

WOBJS:=	sg.win32_o sg_widget.win32_o sg_texture.win32_o \
	sg_alignment.win32_o sg_table.win32_o sg_scrollable.win32_o \
	sg_panel.win32_o sg_animatedpanel.win32_o \
	sg_textarea.win32_o sg_progressbar.win32_o sg_editable.win32_o \
	sg_button.win32_o sg_stickybutton.win32_o sg_menu.win32_o sg_dragable.win32_o\
	sg_compound.win32_o sg_filebrowser.win32_o sg_multitab.win32_o sg_listbox.win32_o \
	sg_radiobuttons.win32_o sg_combobox.win32_o sg_pulldown.win32_o sg_sliderbar.win32_o \
	sg_scrollingarea.win32_o sg_dndboxes.win32_o sg_tabs.win32_o \
	sg_passthrough.win32_o sg_translabel.win32_o

all: $(OBJS)

ChangeLog:	*.cpp *.h TODO COPYING Makefile CVS scripts/* tests/*
	cvs2cl.pl --hide-filenames --no-wrap --stdout | tr '\t' '@' | sed 's-^@-@* -g' | tr '@' '\t' | sed 's-stea-sgreene-g' > ChangeLog

%.o:	%.cpp
	$(CXX) $(CXXFLAGS) -c $<

%.o:	tests/%.cpp
	$(CXX) $(CXXFLAGS) -c $<

%.win32_o:	%.cpp %.o
	$(WCXX) $(WCXXFLAGS) -c $< -o $@

%.win32_o:	tests/%.cpp %.o
	$(WCXX) $(WCXXFLAGS) -c $< -o $@

deps.mk:	*.cpp *.h
	$(CXX) $(CXXFLAGS) -MM *.cpp tests/*.cpp > deps.mk

clean:
	rm -f deps.mk *.o test1 test2 *.win32_o *.exe

test:	test1 test2

test1:	test1.o renderer.o audio.o $(OBJS)
	$(CXX) $(CXXFLAGS) -o test1 test1.o renderer.o audio.o $(OBJS) $(LIBS)

test2:	test2.o renderer.o audio.o $(OBJS)
	$(CXX) $(CXXFLAGS) -o test2 test2.o renderer.o audio.o $(OBJS) $(LIBS)

win32:	$(WOBJS)

wintest:	test1.exe test2.exe

test1.exe:	test1.win32_o renderer.win32_o audio.win32_o $(WOBJS)
	$(WCXX) $(WCXXFLAGS) -o test1.exe test1.win32_o renderer.win32_o audio.win32_o $(WOBJS) $(WLIBS)

test2.exe:	test2.win32_o renderer.win32_o audio.win32_o $(WOBJS)
	$(WCXX) $(WCXXFLAGS) -o test2.exe test2.win32_o renderer.win32_o audio.win32_o $(WOBJS) $(WLIBS)

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
