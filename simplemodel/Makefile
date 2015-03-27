# *************************************************************************
#  This file is part of the SimpleModel Example Module by Steaphan Greene
#
#  Copyright 2005-2015 Steaphan Greene <steaphan@gmail.com>
#
#  SimpleModel is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 3 of the License, or
#  (at your option) any later version.
#
#  SimpleModel is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with SimpleModel (see the file named "COPYING");
#  If not, see <http://www.gnu.org/licenses/>.
#
# *************************************************************************

ARCH = $(shell gcc -v 2>&1 | grep Target | cut -f2 -d" ")

all:	$(ARCH)/libsimplemodel.a

win32:	libsimplemodel.win32_a

#All of this is usually overridden by the main Makefile
CXX=	g++
FLAGS=	--std=c++11 -g -Wall -Werror -DSDL_DEBUG=SDL_INIT_NOPARACHUTE `pkg-config sdl2 gl --cflags` `pkg-config zziplib --cflags`
LIBS=	`pkg-config sdl2 gl --libs` `pkg-config zziplib --libs` -lOVR -ldl -lX11 -lm -lXrandr -lpthread -lrt -lGL -lGLU -lGLEW
WCXX=	i586-mingw32msvc-g++
WFLAGS=	-s -O2 -Wall -Werror `/opt/i586-mingw32msvc/bin/pkg-config sdl2 gl --cflags`
WLIBS=	`/opt/i586-mingw32msvc/bin/pkg-config sdl2 gl --libs` -lSDL -lpng -ljpeg -lpng `/opt/i586-mingw32msvc/bin/pkg-config zziplib --libs` -lopengl32 -lglu32

OBJS:=	$(ARCH)/simplemodel.o $(ARCH)/simplemodel_q3dir.o \
	$(ARCH)/simplemodel_md3.o $(ARCH)/simplemodel_md2.o \
	$(ARCH)/simplemodel_pmx.o $(ARCH)/simplemodel_pmd.o \
	$(ARCH)/simplemodel_md.o $(ARCH)/simplemodel_mdx.o \
	$(ARCH)/simplemodel_sphere.o $(ARCH)/simplemodel_wedge.o \
	$(ARCH)/simplemodel_cone.o $(ARCH)/simplemodel_cube.o \
	$(ARCH)/simplemodel_xyplane.o $(ARCH)/simplemodel_xzplane.o \
	$(ARCH)/simplemodel_yzplane.o $(ARCH)/simplemodel_3ds.o \
	$(ARCH)/simplemodel_obj.o
CXXFLAGS:=	$(FLAGS) --std=c++11 -I../simpletexture -I../simplevideo -I../simplescene -I../simplegui -I../simpleaudio 
CXXLIBS:=	-lSDL2_ttf -lSDL2_image ../simpletexture/$(ARCH)/libsimpletexture.a ../simplevideo/$(ARCH)/libsimplevideo.a ../simplescene/$(ARCH)/libsimplescene.a ../simplegui/$(ARCH)/libsimplegui.a $(LIBS)

WOBJS:=	simplemodel.win32_o simplemodel_q3dir.win32_o \
	simplemodel_md3.win32_o simplemodel_md2.win32_o \
	simplemodel_pmx.win32_o simplemodel_pmd.win32_o \
	simplemodel_md.win32_o simplemodel_mdx.win32_o \
	simplemodel_sphere.win32_o simplemodel_wedge.win32_o \
	simplemodel_cone.win32_o simplemodel_cube.win32_o \
	simplemodel_xyplane.win32_o simplemodel_xzplane.win32_o \
	simplemodel_yzplane.win32_o \
	simplemodel_3ds.win32_o simplemodel_obj.win32_o
WCXXFLAGS:=	$(WFLAGS) -I../simpletexture -I../simplevideo -I../simplescene -I../simplegui
WCXXLIBS:=	-lSDL2_ttf -lSDL2_image $(WLIBS) ../simpletexture/$(ARCH)/libsimpletexture.a ../simplevideo/$(ARCH)/libsimplevideo.a ../simplescene/$(ARCH)/libsimplescene.a ../simplegui/$(ARCH)/libsimplegui.a

$(ARCH):
	mkdir -p $(ARCH)

$(ARCH)/libsimplemodel.a:	$(OBJS) | $(ARCH)
	rm -vf $@
	ar rcs $@ $(OBJS)

libsimplemodel.win32_a:	$(WOBJS)
	rm -vf $@
	i586-mingw32msvc-ar rcs $@ $(WOBJS)

ChangeLog:	*.cpp *.h TODO COPYING Makefile tests/*.cpp tests/*.h .svn
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

test:	test0 test1 test2

test0:	$(ARCH)/test0.o $(ARCH)/libsimplemodel.a ../simpletexture/$(ARCH)/libsimpletexture.a ../simplevideo/$(ARCH)/libsimplevideo.a ../simplescene/$(ARCH)/libsimplescene.a ../simplegui/$(ARCH)/libsimplegui.a ../simpleaudio/$(ARCH)/libsimpleaudio.a

	$(CXX) $(CXXFLAGS) -o test0 $(ARCH)/test0.o $(ARCH)/libsimplemodel.a $(CXXLIBS) ../simpleaudio/$(ARCH)/libsimpleaudio.a -lSDL2_mixer

test1:	$(ARCH)/test1.o $(ARCH)/libsimplemodel.a ../simpletexture/$(ARCH)/libsimpletexture.a ../simplevideo/$(ARCH)/libsimplevideo.a ../simplescene/$(ARCH)/libsimplescene.a ../simplegui/$(ARCH)/libsimplegui.a ../simpleaudio/$(ARCH)/libsimpleaudio.a

	$(CXX) $(CXXFLAGS) -o test1 $(ARCH)/test1.o $(ARCH)/libsimplemodel.a $(CXXLIBS) ../simpleaudio/$(ARCH)/libsimpleaudio.a -lSDL2_mixer

test2:	$(ARCH)/test2.o $(ARCH)/libsimplemodel.a ../simpletexture/$(ARCH)/libsimpletexture.a ../simplevideo/$(ARCH)/libsimplevideo.a ../simplescene/$(ARCH)/libsimplescene.a ../simplegui/$(ARCH)/libsimplegui.a ../simpleaudio/$(ARCH)/libsimpleaudio.a
	$(CXX) $(CXXFLAGS) -o test2 $(ARCH)/test2.o $(ARCH)/libsimplemodel.a $(CXXLIBS) ../simpleaudio/$(ARCH)/libsimpleaudio.a -lSDL2_mixer

win32_test:	test0.exe test1.exe test2.exe
wintest:	win32_test

test0.exe:	test0.win32_o libsimplemodel.win32_a
	$(WCXX) $(WCXXFLAGS) -o test0.exe test0.win32_o libsimplemodel.win32_a $(WCXXLIBS)

test1.exe:	test1.win32_o libsimplemodel.win32_a
	$(WCXX) $(WCXXFLAGS) -o test1.exe test1.win32_o libsimplemodel.win32_a $(WCXXLIBS)

test2.exe:	test2.win32_o libsimplemodel.win32_a
	$(WCXX) $(WCXXFLAGS) -o test2.exe test2.win32_o libsimplemodel.win32_a $(WCXXLIBS)

TSTR:=	$(shell date -u +"%Y%m%d%H%M")
backup:	tar
tar:
	cd .. ; tar chvf simplemodel/simplemodel.$(TSTR).tar \
		simplemodel/Makefile simplemodel/TODO simplemodel/COPYING \
		simplemodel/tests/*.cpp simplemodel/tests/*.h \
		simplemodel/*.cpp simplemodel/*.h
#		simplemodel/scripts/*.* simplemodel/deps.$(ARCH).mk \
	gzip -9 simplemodel.$(TSTR).tar

include deps.$(ARCH).mk
