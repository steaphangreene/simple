# *************************************************************************
#  This file is part of the SimpleGUI Example Module by Steaphan Greene
#
#  Copyright 2005-2008 Steaphan Greene <stea@cs.binghamton.edu>
#
#  SimpleGUI is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 3 of the License, or
#  (at your option) any later version.
#
#  SimpleGUI is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with SimpleGUI (see the file named "COPYING");
#  If not, see <http://www.gnu.org/licenses/>.
#  
# *************************************************************************

ARCH = $(shell gcc -v 2>&1 | grep Target | cut -f2 -d" ")

all:	$(ARCH)/libsimplegui.a

win32:	libsimplegui.win32_a

#all:	test

#All of this is usually overridden by the main Makefile
CXX=	g++
FLAGS=	-g -Wall -DSDL_DEBUG=SDL_INIT_NOPARACHUTE `pkg-config sdl --cflags` `pkg-config zziplib --cflags`
LIBS=	`pkg-config sdl --libs` `pkg-config zziplib --libs` -lGL -lGLU
WCXX=	i586-mingw32msvc-g++
WFLAGS=	-s -O2 -Wall `/opt/i586-mingw32msvc/bin/pkg-config sdl --cflags`
WLIBS=	`/opt/i586-mingw32msvc/bin/pkg-config sdl --libs` -lSDL -lpng -ljpeg -lpng `/opt/i586-mingw32msvc/bin/pkg-config zziplib --libs` -lopengl32 -lglu32

OBJS:=	$(ARCH)/sg.o $(ARCH)/sg_widget.o $(ARCH)/sg_ranger.o \
	$(ARCH)/sg_ranger2d.o $(ARCH)/sg_alignment.o \
	$(ARCH)/sg_positional.o $(ARCH)/sg_table.o $(ARCH)/sg_aspecttable.o \
	$(ARCH)/sg_scrollable.o $(ARCH)/sg_autoscroll.o \
	$(ARCH)/sg_panel.o $(ARCH)/sg_animatedpanel.o \
	$(ARCH)/sg_textarea.o $(ARCH)/sg_progressbar.o \
	$(ARCH)/sg_editable.o $(ARCH)/sg_button.o \
	$(ARCH)/sg_stickybutton.o $(ARCH)/sg_menu.o \
	$(ARCH)/sg_dragable.o $(ARCH)/sg_compound.o \
	$(ARCH)/sg_filebrowser.o $(ARCH)/sg_multitab.o \
	$(ARCH)/sg_listbox.o $(ARCH)/sg_radiobuttons.o \
	$(ARCH)/sg_combobox.o $(ARCH)/sg_pulldown.o \
	$(ARCH)/sg_sliderbar.o $(ARCH)/sg_scrollingarea.o \
	$(ARCH)/sg_dndboxes.o $(ARCH)/sg_tabs.o \
	$(ARCH)/sg_passthrough.o $(ARCH)/sg_translabel.o \
	$(ARCH)/sg_spinner.o
CXXFLAGS:=	$(FLAGS) -I../simpletexture -I../simplevideo
CXXLIBS:=	-lSDL_ttf -lSDL_image $(LIBS) ../simpletexture/$(ARCH)/libsimpletexture.a ../simplevideo/$(ARCH)/libsimplevideo.a

WOBJS:=	sg.win32_o sg_widget.win32_o sg_ranger.win32_o sg_ranger2d.win32_o \
	sg_alignment.win32_o sg_positional.win32_o sg_table.win32_o \
	sg_aspecttable.win32_o sg_scrollable.win32_o sg_autoscroll.win32_o \
	sg_panel.win32_o sg_animatedpanel.win32_o \
	sg_textarea.win32_o sg_progressbar.win32_o sg_editable.win32_o \
	sg_button.win32_o sg_stickybutton.win32_o sg_menu.win32_o sg_dragable.win32_o\
	sg_compound.win32_o sg_filebrowser.win32_o sg_multitab.win32_o sg_listbox.win32_o \
	sg_radiobuttons.win32_o sg_combobox.win32_o sg_pulldown.win32_o sg_sliderbar.win32_o \
	sg_scrollingarea.win32_o sg_dndboxes.win32_o sg_tabs.win32_o \
	sg_passthrough.win32_o sg_translabel.win32_o sg_spinner.win32_o
WCXXFLAGS:=	$(WFLAGS) -I../simpletexture -I../simplevideo
WCXXLIBS:=	-lSDL_ttf -lSDL_image $(WLIBS) ../simpletexture/$(ARCH)/libsimpletexture.a ../simplevideo/$(ARCH)/libsimplevideo.a

$(ARCH):
	mkdir -p $(ARCH)

$(ARCH)/libsimplegui.a:	$(OBJS) | $(ARCH)
	rm -vf $@
	ar rcs $@ $(OBJS)

libsimplegui.win32_a:	$(WOBJS)
	rm -vf $@
	i586-mingw32msvc-ar rcs $@ $(WOBJS)

ChangeLog:	*.cpp *.h TODO COPYING Makefile tests/*.cpp tests/*.h scripts/* .svn
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
#	$(CXX) $(CXXFLAGS) -MM *.cpp | sed "s|^\(.*\)\.o:|x86_64-linux-gnu/\1.o:|" > $@

clean:
	rm -f deps.$(ARCH).mk $(ARCH)/*.o $(ARCH)/*.a *.win32_o *.win32_a test[0-9] *.exe ChangeLog
	mkdir -p $(ARCH)	# So rmdir doesn't fail
	rmdir $(ARCH)

test:	test1 test2 test3 test4 test5

test1:	$(ARCH)/test1.o $(ARCH)/renderer.o $(ARCH)/audio.o $(ARCH)/libsimplegui.a
	$(CXX) $(CXXFLAGS) -o test1 $(ARCH)/test1.o $(ARCH)/renderer.o $(ARCH)/audio.o $(ARCH)/libsimplegui.a $(CXXLIBS)

test2:	$(ARCH)/test2.o $(ARCH)/renderer.o $(ARCH)/audio.o $(ARCH)/libsimplegui.a
	$(CXX) $(CXXFLAGS) -o test2 $(ARCH)/test2.o $(ARCH)/renderer.o $(ARCH)/audio.o $(ARCH)/libsimplegui.a $(CXXLIBS)

test3:  $(ARCH)/test3.o $(ARCH)/renderer.o $(ARCH)/audio.o $(ARCH)/libsimplegui.a
	$(CXX) $(CXXFLAGS) -o test3 $(ARCH)/test3.o $(ARCH)/renderer.o $(ARCH)/audio.o $(ARCH)/libsimplegui.a $(CXXLIBS)
	
test4:  $(ARCH)/test4.o $(ARCH)/renderer.o $(ARCH)/audio.o $(ARCH)/libsimplegui.a
	$(CXX) $(CXXFLAGS) -o test4 $(ARCH)/test4.o $(ARCH)/renderer.o $(ARCH)/audio.o $(ARCH)/libsimplegui.a $(CXXLIBS)
	
test5:  $(ARCH)/test5.o $(ARCH)/renderer.o $(ARCH)/audio.o $(ARCH)/libsimplegui.a
	$(CXX) $(CXXFLAGS) -o test5 $(ARCH)/test5.o $(ARCH)/renderer.o $(ARCH)/audio.o $(ARCH)/libsimplegui.a $(CXXLIBS)
	
win32_test:	test1.exe test2.exe test3.exe test4.exe test5.exe
wintest:	win32_test

test1.exe:	test1.win32_o renderer.win32_o audio.win32_o libsimplegui.win32_a
	$(WCXX) $(WCXXFLAGS) -o test1.exe test1.win32_o renderer.win32_o audio.win32_o libsimplegui.win32_a $(WCXXLIBS)

test2.exe:	test2.win32_o renderer.win32_o audio.win32_o libsimplegui.win32_a
	$(WCXX) $(WCXXFLAGS) -o test2.exe test2.win32_o renderer.win32_o audio.win32_o libsimplegui.win32_a $(WCXXLIBS)

test3.exe:	test3.win32_o renderer.win32_o audio.win32_o libsimplegui.win32_a
	$(WCXX) $(WCXXFLAGS) -o test3.exe test3.win32_o renderer.win32_o audio.win32_o libsimplegui.win32_a $(WCXXLIBS)
	
test4.exe:	test4.win32_o renderer.win32_o audio.win32_o libsimplegui.win32_a
	$(WCXX) $(WCXXFLAGS) -o test4.exe test4.win32_o renderer.win32_o audio.win32_o libsimplegui.win32_a $(WCXXLIBS)
	
test5.exe:	test5.win32_o renderer.win32_o audio.win32_o libsimplegui.win32_a
	$(WCXX) $(WCXXFLAGS) -o test5.exe test5.win32_o renderer.win32_o audio.win32_o libsimplegui.win32_a $(WCXXLIBS)
	
TSTR:=	$(shell date -u +"%Y%m%d%H%M")
backup:	tar
tar:
	cd .. ; tar chvf simplegui/simplegui.$(TSTR).tar \
		simplegui/Makefile simplegui/TODO simplegui/COPYING \
		simplegui/*.cpp simplegui/*.h \
		simplegui/tests/*.cpp simplegui/tests/*.h \
		simplegui/scripts/*.* simplegui/deps.$(ARCH).mk
	gzip -9 simplegui.$(TSTR).tar

include deps.$(ARCH).mk
