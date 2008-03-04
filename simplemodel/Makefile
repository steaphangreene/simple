# *************************************************************************
#  This file is part of the SimpleModel Example Module by Steaphan Greene
#
#  Copyright 2005-2008 Steaphan Greene <stea@cs.binghamton.edu>
#
#  SimpleModel is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  SimpleModel is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with SimpleModel (see the file named "COPYING");
#  if not, write to the the Free Software Foundation, Inc.,
#  59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
# *************************************************************************

all:	libsimplemodel.a

win32:	libsimplemodel.win32_a

#All of this is usually overridden by the main Makefile
CXX=	g++
FLAGS=	-g -Wall -DSDL_DEBUG=SDL_INIT_NOPARACHUTE `sdl-config --cflags` `zzip-config --cflags`
LIBS=	`sdl-config --libs` `zzip-config --libs` -lGL -lGLU
WCXX=	i586-mingw32msvc-g++
WFLAGS=	-s -O2 -Wall `/usr/i586-mingw32msvc/bin/i586-mingw32msvc-sdl-config --cflags`
WLIBS=	`/usr/i586-mingw32msvc/bin/i586-mingw32msvc-sdl-config --libs` -lSDL -lpng -ljpeg -lpng `/usr/i586-mingw32msvc/bin/i586-mingw32msvc-zzip-config --libs` -lopengl32 -lglu32

OBJS:=	simplemodel.o simplemodel_q3dir.o \
	simplemodel_md3.o simplemodel_md2.o \
	simplemodel_md.o simplemodel_mdx.o \
	simplemodel_sphere.o simplemodel_wedge.o \
	simplemodel_cone.o simplemodel_cube.o \
	simplemodel_3ds.o simplemodel_obj.o
CXXFLAGS:=	$(FLAGS) -I../simpletexture
CXXLIBS:=	-lSDL_ttf -lSDL_image $(LIBS) ../simpletexture/libsimpletexture.a

WOBJS:=	simplemodel.win32_o simplemodel_q3dir.win32_o \
	simplemodel_md3.win32_o simplemodel_md2.win32_o \
	simplemodel_md.win32_o simplemodel_mdx.win32_o \
	simplemodel_sphere.win32_o simplemodel_wedge.win32_o \
	simplemodel_cone.win32_o simplemodel_cube.win32_o \
	simplemodel_3ds.win32_o simplemodel_obj.win32_o
WCXXFLAGS:=	$(WFLAGS) -I../simpletexture
WCXXLIBS:=	-lSDL_ttf -lSDL_image $(WLIBS) ../simpletexture/libsimpletexture.a

libsimplemodel.a:	$(OBJS)
	rm -vf $@
	ar rcs $@ $(OBJS)

libsimplemodel.win32_a:	$(WOBJS)
	rm -vf $@
	i586-mingw32msvc-ar rcs $@ $(WOBJS)

ChangeLog:	*.cpp *.h TODO COPYING Makefile tests/*.cpp tests/*.h .svn
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

test:	test1

test1:	test1.o renderer.o libsimplemodel.a
	$(CXX) $(CXXFLAGS) -o test1 test1.o renderer.o libsimplemodel.a $(CXXLIBS)
	
win32_test:	test1.exe
wintest:	win32_test

test1.exe:	test1.win32_o renderer.win32_o libsimplemodel.win32_a
	$(WCXX) $(WCXXFLAGS) -o test1.exe test1.win32_o renderer.win32_o libsimplemodel.win32_a $(WCXXLIBS)
	
TSTR:=	$(shell date -u +"%Y%m%d%H%M")
backup:	tar
tar:
	cd .. ; tar chvf simplemodel/simplemodel.$(TSTR).tar \
		simplemodel/Makefile simplemodel/TODO simplemodel/COPYING \
		simplemodel/tests/*.cpp simplemodel/tests/*.h \
		simplemodel/*.cpp simplemodel/*.h
#		simplemodel/scripts/*.* simplemodel/deps.mk \
	gzip -9 simplemodel.$(TSTR).tar

include deps.mk
