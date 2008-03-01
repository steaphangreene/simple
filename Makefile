# *************************************************************************
#  This file is part of the Simple* Example Suite by Steaphan Greene
#
#  Copyright 2005-2008 Steaphan Greene <stea@cs.binghamton.edu>
#
#  Simple* is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  Simple* is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with Simple* (see the file named "COPYING");
#  if not, write to the the Free Software Foundation, Inc.,
#  59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
# *************************************************************************

DESTDIR =
PREFIX = $(DESTDIR)/usr/local
BINDIR = $(PREFIX)/bin
LIBDIR = $(PREFIX)/lib/simple
INCDIR = $(PREFIX)/include/simple
WLIBDIR = $(PREFIX)/i586-mingw32msvc/lib/simple
SVNREV = $(shell svn info | grep Revision | cut -f2 -d" ")
REVISION = $(shell printf "%.10d" $(SVNREV))

INSTALL = /usr/bin/install

#PRODUCTION OPTIONS (STANDARD)
#CXX=	g++
#FLAGS=	-s -O2 -Wall `sdl-config --cflags` `zzip-config --cflags`
#LIBS=	`sdl-config --libs` `zzip-config --libs` -lGL -lGLU

#PRODUCTION OPTIONS (WORKAROUND FOR MacOS-X)
#CXX=	g++
#FLAGS=	-s -O2 -Wall `sdl-config --cflags` `zzip-config --cflags`
#LIBS=	`sdl-config --libs` `zzip-config --libs` -framework OpenGL

#PRODUCTION OPTIONS (WORKAROUND FOR CYGWIN)
#CXX=	g++
#FLAGS=	-s -O2 -Wall `sdl-config --cflags` `zzip-config --cflags`
#LIBS=	`sdl-config --libs` `zzip-config --libs` -L/usr/X11R6/bin -lopengl32 -lglu32

#DEGUGGING OPTIONS (NO EFENCE)
CXX=	g++
FLAGS=	-g -Wall -DSDL_DEBUG=SDL_INIT_NOPARACHUTE `sdl-config --cflags` `zzip-config --cflags`
LIBS=	`sdl-config --libs` `zzip-config --libs` -lGL -lGLU

#DEGUGGING OPTIONS (WITH EFENCE)
#CXX=	g++
#FLAGS=	-g -Wall -DSDL_DEBUG=SDL_INIT_NOPARACHUTE `sdl-config --cflags` `zzip-config --cflags`
#LIBS=	`sdl-config --libs` `zzip-config --libs` -lefence -lGL -lGLU

#PROFILING OPTIONS
#CXX=	g++
#FLAGS=	-pg -g -Wall -DSDL_DEBUG=SDL_INIT_NOPARACHUTE `sdl-config --cflags` `zzip-config --cflags`
#LIBS=	`sdl-config --libs` `zzip-config --libs` -lGL -lGLU

#PRODUCTION OPTIONS (CROSS-COMPILED FOR WINDOWS)
WCXX=	i586-mingw32msvc-g++
WFLAGS=	-s -O2 -Wall `/usr/i586-mingw32msvc/bin/i586-mingw32msvc-sdl-config --cflags`
WLIBS=	`/usr/i586-mingw32msvc/bin/i586-mingw32msvc-sdl-config --libs` -lSDL -lpng -ljpeg -lpng `/usr/i586-mingw32msvc/bin/i586-mingw32msvc-zzip-config --libs` -lopengl32 -lglu32

all:	build

build:
	make -C simpleaudio CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)'
	make -C simpletexture CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)'
	make -C simplevideo CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)'
	make -C simplegui CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)'
	make -C simplemodel CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)'
	make -C simpleconnect CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)'
	make -C simpleconfig CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)'

install:	all
	$(INSTALL) -d $(LIBDIR)
	$(INSTALL) -d $(INCDIR)
	$(INSTALL) -d $(BINDIR)
	$(INSTALL) -m 644 simple*/lib*.a $(LIBDIR)
	$(INSTALL) -m 644 simple*/*.h $(INCDIR)
	cat scripts/simple-config.sh | sed 's-/usr/local-$(PREFIX)-g' | sed 's-REVISION-$(REVISION)-g' > simple-config
	$(INSTALL) -m 755 simple-config $(BINDIR)

uninstall:	uninstall_win32
	rm -f $(LIBDIR)/libsimple*.a
	rm -f $(INCDIR)/s*.h
	rm -vf $(BINDIR)/simple-config

win32:
	make -C simpleaudio WCXX='$(WCXX)' WFLAGS='$(WFLAGS)' WLIBS='$(WLIBS)' $@
	make -C simpletexture WCXX='$(WCXX)' WFLAGS='$(WFLAGS)' WLIBS='$(WLIBS)' $@
	make -C simplevideo WCXX='$(WCXX)' WFLAGS='$(WFLAGS)' WLIBS='$(WLIBS)' $@
	make -C simplegui WCXX='$(WCXX)' WFLAGS='$(WFLAGS)' WLIBS='$(WLIBS)' $@
	make -C simplemodel WCXX='$(WCXX)' WFLAGS='$(WFLAGS)' WLIBS='$(WLIBS)' $@
	make -C simpleconnect WCXX='$(WCXX)' WFLAGS='$(WFLAGS)' WLIBS='$(WLIBS)' $@
	make -C simpleconfig WCXX='$(WCXX)' WFLAGS='$(WFLAGS)' WLIBS='$(WLIBS)' $@

install_win32:	install win32
	$(INSTALL) -d $(WLIBDIR)
	$(INSTALL) -m 644 simpleaudio/libsimpleaudio.win32_a $(WLIBDIR)/libsimpleaudio.a
	$(INSTALL) -m 644 simpletexture/libsimpletexture.win32_a $(WLIBDIR)/libsimpletexture.a
	$(INSTALL) -m 644 simplevideo/libsimplevideo.win32_a $(WLIBDIR)/libsimplevideo.a
	$(INSTALL) -m 644 simplegui/libsimplegui.win32_a $(WLIBDIR)/libsimplegui.a
	$(INSTALL) -m 644 simplemodel/libsimplemodel.win32_a $(WLIBDIR)/libsimplemodel.a
	$(INSTALL) -m 644 simpleconnect/libsimpleconnect.win32_a $(WLIBDIR)/libsimpleconnect.a
	$(INSTALL) -m 644 simpleconfig/libsimpleconfig.win32_a $(WLIBDIR)/libsimpleconfig.a
	cat scripts/simple-config.sh | sed 's|prefix=".*"|prefix="$(PREFIX)"|g' | sed 's|cross_prefix=".*"|cross_prefix="i586-mingw32msvc-"|g' | sed 's|cross_dir=".*"|cross_dir="/i586-mingw32msvc"|g' | sed 's|-lGL -lGLU|-lopengl32 -lglu32|g' | sed 's|base_libs=".*"|base_libs="-lSDL_net -lwsock32 -lSDL_ttf -lSDL_image -lvorbisfile -lvorbis -logg -lSDL -lpng -ljpeg -lpng"|g' > i586-mingw32msvc-simple-config
	$(INSTALL) -m 755 i586-mingw32msvc-simple-config $(BINDIR)

win32_install:	install_win32

uninstall_win32:
	rm -vf $(WLIBDIR)/libsimple*.win32_a
	rm -vf $(BINDIR)/i586-mingw32msvc-simple-config

win32_uninstall:	uninstall_win32

ChangeLog:	.svn
	make -C simpleaudio CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
	make -C simpletexture CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
	make -C simplevideo CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
	make -C simplegui CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
	make -C simplemodel CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
	make -C simpleconnect CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
	make -C simpleconfig CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
	./scripts/svn2cl.sh | sed 's-  stea-  sgreene-g' > ChangeLog

test:	
	make -C simpleaudio CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)'	#No tests!
	make -C simpletexture CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
	make -C simplevideo CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)'	#No tests!
	make -C simplegui CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
	make -C simplemodel CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
	make -C simpleconnect CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)'	#No tests!
	make -C simpleconfig CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@

win32_test:	
	make -C simpleaudio WCXX='$(WCXX)' WFLAGS='$(WFLAGS)' WLIBS='$(WLIBS)' win32	#No tests!
	make -C simpletexture WCXX='$(WCXX)' WFLAGS='$(WFLAGS)' WLIBS='$(WLIBS)' $@
	make -C simplevideo WCXX='$(WCXX)' WFLAGS='$(WFLAGS)' WLIBS='$(WLIBS)' win32	#No tests!
	make -C simplegui WCXX='$(WCXX)' WFLAGS='$(WFLAGS)' WLIBS='$(WLIBS)' $@
	make -C simplemodel WCXX='$(WCXX)' WFLAGS='$(WFLAGS)' WLIBS='$(WLIBS)' $@
	make -C simpleconnect WCXX='$(WCXX)' WFLAGS='$(WFLAGS)' WLIBS='$(WLIBS)' win32	#No tests!
	make -C simpleconfig WCXX='$(WCXX)' WFLAGS='$(WFLAGS)' WLIBS='$(WLIBS)' $@

wintest:	win32_test
test_win32:	win32_test

clean:
	make -C simpleaudio CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
	make -C simpletexture CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
	make -C simplevideo CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
	make -C simplegui CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
	make -C simplemodel CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
	make -C simpleconnect CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
	make -C simpleconfig CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
	rm -f *simple-config ChangeLog

backup:
	make -C simpleaudio CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
	make -C simpletexture CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
	make -C simplevideo CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
	make -C simplegui CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
	make -C simplemodel CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
	make -C simpleconnect CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
	make -C simpleconfig CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@

tar:
	make -C simpleaudio CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
	make -C simpletexture CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
	make -C simplevideo CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
	make -C simplegui CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
	make -C simplemodel CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
	make -C simpleconnect CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
	make -C simpleconfig CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
