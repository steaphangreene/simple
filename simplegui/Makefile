# *************************************************************************
#  This file is part of the SimpleGUI Example Module by Steaphan Greene
#
#  Copyright 2005-2006 Steaphan Greene <stea@cs.binghamton.edu>
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

all:	libsimplegui.a

win32:	libsimplegui.win32_a

#all:	test

#PRODUCTION OPTIONS (STANDARD)
#CXX=	g++
#CXXFLAGS=	-s -O2 -Wall `sdl-config --cflags`
#LIBS=	../simpletexture/libsimpletexture.a `sdl-config --libs` -lSDL_image -lSDL_ttf -lGL -lGLU

#PRODUCTION OPTIONS (WORKAROUND FOR MacOS-X)
#CXX=	g++
#CXXFLAGS=	-s -O2 -Wall `sdl-config --cflags`
#LIBS=	../simpletexture/libsimpletexture.a `sdl-config --libs` -lSDL_image -lSDL_ttf -framework OpenGL

#PRODUCTION OPTIONS (WORKAROUND FOR CYGWIN)
#CXX=	g++
#CXXFLAGS=	-s -O2 -Wall `sdl-config --cflags`
#LIBS=	../simpletexture/libsimpletexture.a `sdl-config --libs` -lSDL_image -lSDL_ttf -L/usr/X11R6/bin -lopengl32 -lglu32

#DEGUGGING OPTIONS (NO EFENCE)
CXX=	g++
CXXFLAGS=	-g -Wall -DSDL_DEBUG=SDL_INIT_NOPARACHUTE `sdl-config --cflags`
LIBS=	../simpletexture/libsimpletexture.a `sdl-config --libs` -lSDL_image -lSDL_ttf -lGL -lGLU

#DEGUGGING OPTIONS (WITH EFENCE)
#CXX=	g++
#CXXFLAGS=	-g -Wall -DSDL_DEBUG=SDL_INIT_NOPARACHUTE `sdl-config --cflags`
#LIBS=	../simpletexture/libsimpletexture.a `sdl-config --libs` -lefence -lSDL_image -lSDL_ttf -lGL -lGLU

#PROFILING OPTIONS
#CXX=	g++
#CXXFLAGS=	-pg -g -Wall -DSDL_DEBUG=SDL_INIT_NOPARACHUTE `sdl-config --cflags`
#LIBS=	../simpletexture/libsimpletexture.a `sdl-config --libs` -lSDL_image -lSDL_ttf -lGL -lGLU

OBJS:=	sg.o sg_widget.o sg_ranger.o sg_ranger2d.o \
	sg_alignment.o sg_positional.o sg_table.o \
	sg_scrollable.o sg_autoscroll.o \
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
WLIBS=	../simpletexture/libsimpletexture.win32_a `/usr/i586-mingw32msvc/bin/i586-mingw32msvc-sdl-config --libs` -lSDL_ttf -lSDL_image -lSDL -lglu32 -lpng -ljpeg -lz -lpng -lopengl32

WOBJS:=	sg.win32_o sg_widget.win32_o sg_ranger.win32_o sg_ranger2d.win32_o \
	sg_alignment.win32_o sg_positional.win32_o sg_table.win32_o \
	sg_scrollable.win32_o sg_autoscroll.win32_o \
	sg_panel.win32_o sg_animatedpanel.win32_o \
	sg_textarea.win32_o sg_progressbar.win32_o sg_editable.win32_o \
	sg_button.win32_o sg_stickybutton.win32_o sg_menu.win32_o sg_dragable.win32_o\
	sg_compound.win32_o sg_filebrowser.win32_o sg_multitab.win32_o sg_listbox.win32_o \
	sg_radiobuttons.win32_o sg_combobox.win32_o sg_pulldown.win32_o sg_sliderbar.win32_o \
	sg_scrollingarea.win32_o sg_dndboxes.win32_o sg_tabs.win32_o \
	sg_passthrough.win32_o sg_translabel.win32_o

libsimplegui.a:	$(OBJS)
	ar rcs libsimplegui.a $(OBJS)

libsimplegui.win32_a:	$(WOBJS)
	i586-mingw32msvc-ar rcs libsimplegui.win32_a $(WOBJS)

ChangeLog:	*.cpp *.h TODO COPYING Makefile .svn scripts/* tests/*
	./scripts/svn2cl.sh | sed 's-stea-sgreene-g' > ChangeLog

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
	rm -f deps.mk *.o *.a test[0-9] *.win32_o *.win32_a *.exe

test:	test1 test2 test3 test4

test1:	test1.o renderer.o audio.o libsimplegui.a
	$(CXX) $(CXXFLAGS) -o test1 test1.o renderer.o audio.o libsimplegui.a $(LIBS)

test2:	test2.o renderer.o audio.o libsimplegui.a
	$(CXX) $(CXXFLAGS) -o test2 test2.o renderer.o audio.o libsimplegui.a $(LIBS)

test3:  test3.o renderer.o audio.o libsimplegui.a
	$(CXX) $(CXXFLAGS) -o test3 test3.o renderer.o audio.o libsimplegui.a $(LIBS)
	
test4:  test4.o renderer.o audio.o libsimplegui.a
	$(CXX) $(CXXFLAGS) -o test4 test4.o renderer.o audio.o libsimplegui.a $(LIBS)
	
win32_test:	test1.exe test2.exe test3.exe test4.exe
wintest:	win32_test

test1.exe:	test1.win32_o renderer.win32_o audio.win32_o libsimplegui.win32_a
	$(WCXX) $(WCXXFLAGS) -o test1.exe test1.win32_o renderer.win32_o audio.win32_o libsimplegui.win32_a $(WLIBS)

test2.exe:	test2.win32_o renderer.win32_o audio.win32_o libsimplegui.win32_a
	$(WCXX) $(WCXXFLAGS) -o test2.exe test2.win32_o renderer.win32_o audio.win32_o libsimplegui.win32_a $(WLIBS)

test3.exe:	test3.win32_o renderer.win32_o audio.win32_o libsimplegui.win32_a
	$(WCXX) $(WCXXFLAGS) -o test3.exe test3.win32_o renderer.win32_o audio.win32_o libsimplegui.win32_a $(WLIBS)
	
test4.exe:	test4.win32_o renderer.win32_o audio.win32_o libsimplegui.win32_a
	$(WCXX) $(WCXXFLAGS) -o test4.exe test4.win32_o renderer.win32_o audio.win32_o libsimplegui.win32_a $(WLIBS)
	
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
