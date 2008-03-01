# *************************************************************************
#  This file is part of the SimpleGUI Example Module by Steaphan Greene
#
#  Copyright 2005-2008 Steaphan Greene <stea@cs.binghamton.edu>
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

#All of this is usually overridden by the main Makefile
CXX=	g++
FLAGS=	-g -Wall -DSDL_DEBUG=SDL_INIT_NOPARACHUTE `sdl-config --cflags` `zzip-config --cflags`
LIBS=	`sdl-config --libs` `zzip-config --libs` -lGL -lGLU
WCXX=	i586-mingw32msvc-g++
WFLAGS=	-s -O2 -Wall `/usr/i586-mingw32msvc/bin/i586-mingw32msvc-sdl-config --cflags`
WLIBS=	`/usr/i586-mingw32msvc/bin/i586-mingw32msvc-sdl-config --libs` -lSDL -lpng -ljpeg -lpng `/usr/i586-mingw32msvc/bin/i586-mingw32msvc-zzip-config --libs` -lopengl32 -lglu32

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
CXXFLAGS:=	$(FLAGS) -I../simpletexture
CXXLIBS:=	-lSDL_ttf -lSDL_image $(LIBS) ../simpletexture/libsimpletexture.a

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
WCXXFLAGS:=	$(WFLAGS) -I../simpletexture
WCXXLIBS:=	-lSDL_ttf -lSDL_image $(WLIBS) ../simpletexture/libsimpletexture.a

libsimplegui.a:	$(OBJS)
	rm -vf $@
	ar rcs $@ $(OBJS)

libsimplegui.win32_a:	$(WOBJS)
	rm -vf $@
	i586-mingw32msvc-ar rcs $@ $(WOBJS)

ChangeLog:	*.cpp *.h TODO COPYING Makefile tests/*.cpp tests/*.h scripts/* .svn
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
	$(CXX) $(CXXFLAGS) -MM *.cpp tests/*.cpp > deps.mk

clean:
	rm -f deps.mk *.o *.a *.win32_o *.win32_a test[0-9] *.exe ChangeLog

test:	test1 test2 test3 test4

test1:	test1.o renderer.o audio.o libsimplegui.a
	$(CXX) $(CXXFLAGS) -o test1 test1.o renderer.o audio.o libsimplegui.a $(CXXLIBS)

test2:	test2.o renderer.o audio.o libsimplegui.a
	$(CXX) $(CXXFLAGS) -o test2 test2.o renderer.o audio.o libsimplegui.a $(CXXLIBS)

test3:  test3.o renderer.o audio.o libsimplegui.a
	$(CXX) $(CXXFLAGS) -o test3 test3.o renderer.o audio.o libsimplegui.a $(CXXLIBS)
	
test4:  test4.o renderer.o audio.o libsimplegui.a
	$(CXX) $(CXXFLAGS) -o test4 test4.o renderer.o audio.o libsimplegui.a $(CXXLIBS)
	
win32_test:	test1.exe test2.exe test3.exe test4.exe
wintest:	win32_test

test1.exe:	test1.win32_o renderer.win32_o audio.win32_o libsimplegui.win32_a
	$(WCXX) $(WCXXFLAGS) -o test1.exe test1.win32_o renderer.win32_o audio.win32_o libsimplegui.win32_a $(WCXXLIBS)

test2.exe:	test2.win32_o renderer.win32_o audio.win32_o libsimplegui.win32_a
	$(WCXX) $(WCXXFLAGS) -o test2.exe test2.win32_o renderer.win32_o audio.win32_o libsimplegui.win32_a $(WCXXLIBS)

test3.exe:	test3.win32_o renderer.win32_o audio.win32_o libsimplegui.win32_a
	$(WCXX) $(WCXXFLAGS) -o test3.exe test3.win32_o renderer.win32_o audio.win32_o libsimplegui.win32_a $(WCXXLIBS)
	
test4.exe:	test4.win32_o renderer.win32_o audio.win32_o libsimplegui.win32_a
	$(WCXX) $(WCXXFLAGS) -o test4.exe test4.win32_o renderer.win32_o audio.win32_o libsimplegui.win32_a $(WCXXLIBS)
	
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
