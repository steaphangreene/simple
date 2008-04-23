# *************************************************************************
#  This file is part of the Simple* Example Suite by Steaphan Greene
#
#  Copyright 2005-2008 Steaphan Greene <stea@cs.binghamton.edu>
#
#  Simple* is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 3 of the License, or
#  (at your option) any later version.
#
#  Simple* is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with Simple* (see the file named "COPYING");
#  If not, see <http://www.gnu.org/licenses/>.
#  
# *************************************************************************

DESTDIR =
ARCH = $(shell gcc -v 2>&1 | grep Target | cut -f2 -d" ")
PREFIX = $(DESTDIR)/usr/local
BINDIR = $(PREFIX)/bin
LIBDIR = $(PREFIX)/lib/simple/$(ARCH)
INCDIR = $(PREFIX)/include/simple
WLIBDIR = $(PREFIX)/lib/simple/i586-mingw32msvc
SVNREV = $(shell svn info | grep Revision | cut -f2 -d" ")
REVISION = $(shell printf "%.10d" $(SVNREV))

INSTALL = /usr/bin/install

.PHONY: all
all:	build

#PRODUCTION OPTIONS (STANDARD)
CXX=	$(ARCH)-g++
FLAGS=	-s -O2 -Wall `sdl-config --cflags` `zzip-config --cflags`
LIBS=	`sdl-config --libs` `zzip-config --libs` -lGL -lGLU

#DEGUGGING OPTIONS (NO EFENCE)
debug:
	make FLAGS='-g -Wall -DSDL_DEBUG=SDL_INIT_NOPARACHUTE `sdl-config --cflags` `zzip-config --cflags`' LIBS='`sdl-config --libs` `zzip-config --libs` -lGL -lGLU'

#DEGUGGING OPTIONS (WITH EFENCE)
efence:
	make FLAGS='-g -Wall -DSDL_DEBUG=SDL_INIT_NOPARACHUTE `sdl-config --cflags` `zzip-config --cflags`' LIBS='`sdl-config --libs` `zzip-config --libs` -lefence -lGL -lGLU'

#PROFILING OPTIONS
prof:
	make FLAGS='-pg -g -Wall -DSDL_DEBUG=SDL_INIT_NOPARACHUTE `sdl-config --cflags` `zzip-config --cflags`' LIBS='`sdl-config --libs` `zzip-config --libs` -lGL -lGLU'

#PRODUCTION OPTIONS (WORKAROUND FOR MacOS-X)
macos:
	make FLAGS='-s -O2 -Wall `sdl-config --cflags` `zzip-config --cflags`' LIBS='`sdl-config --libs` `zzip-config --libs` -framework OpenGL'

#PRODUCTION OPTIONS (WORKAROUND FOR CYGWIN)
cygwin:
	make FLAGS='-s -O2 -Wall `sdl-config --cflags` `zzip-config --cflags`' LIBS='`sdl-config --libs` `zzip-config --libs` -L/usr/X11R6/bin -lopengl32 -lglu32'

#PRODUCTION OPTIONS (CROSS-COMPILED FOR WINDOWS)
WARCH=	i586-mingw32msvc
WCXX=	i586-mingw32msvc-g++
WFLAGS=	-s -O2 -Wall `/opt/i586-mingw32msvc/bin/sdl-config --cflags` `/opt/i586-mingw32msvc/bin/zzip-config --cflags` -I/opt/i586-mingw32msvc/include
WLIBS=	`/opt/i586-mingw32msvc/bin/sdl-config --libs` -lSDL -lpng -ljpeg -lpng `/opt/i586-mingw32msvc/bin/zzip-config --libs` -lopengl32 -lglu32

.PHONY: build
build:
	make -C simpleutils ARCH='$(ARCH)' CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)'
	make -C simpleaudio ARCH='$(ARCH)' CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)'
	make -C simpletexture ARCH='$(ARCH)' CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)'
	make -C simplevideo ARCH='$(ARCH)' CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)'
	make -C simplescene ARCH='$(ARCH)' CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)'
	make -C simplegui ARCH='$(ARCH)' CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)'
	make -C simplemodel ARCH='$(ARCH)' CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)'
	make -C simpleconnect ARCH='$(ARCH)' CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)'
	make -C simpleconfig ARCH='$(ARCH)' CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)'

.PHONY: install
install:	all
	$(INSTALL) -d $(LIBDIR)
	$(INSTALL) -d $(INCDIR)
	$(INSTALL) -d $(BINDIR)
	$(INSTALL) -m 644 simple*/$(ARCH)/lib*.a $(LIBDIR)
	$(INSTALL) -m 644 simple*/*.h $(INCDIR)
	cat scripts/simple-config.sh | sed 's-/usr/local-$(PREFIX)-g' | sed 's-REVISION-$(REVISION)-g' > simple-config
	$(INSTALL) -m 755 simple-config $(BINDIR)
	cat simple-config | sed 's|^arch=.*|arch=$(ARCH)|g' > $(ARCH)-simple-config
	$(INSTALL) -m 755 $(ARCH)-simple-config $(BINDIR)

.PHONY: uninstall
uninstall:
	rm -f $(LIBDIR)/libsimple*.a
	rm -f $(INCDIR)/s*.h
	rm -vf $(BINDIR)/simple-config

.PHONY: win32
win32:
	make ARCH=$(WARCH) CXX='$(WCXX)' FLAGS='$(WFLAGS)' LIBS='$(WLIBS)'

.PHONY: install_win32
install_win32:	win32
	make PREFIX=$(PREFIX) ARCH=$(WARCH) CXX='$(WCXX)' FLAGS='$(WFLAGS)' LIBS='$(WLIBS)' install
	$(WARCH)-ranlib $(PREFIX)/lib/simple/$(WARCH)/lib*.a

.PHONY: uninstall_win32
uninstall_win32:	uninstall
	make ARCH=$(WARCH) uninstall

.PHONY: win32_install
win32_install:	install_win32

.PHONY: win32_uninstall
win32_uninstall:	uninstall_win32

ChangeLog:	.svn
	make -C simpleutils ARCH='$(ARCH)' CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
	make -C simpleaudio ARCH='$(ARCH)' CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
	make -C simpletexture ARCH='$(ARCH)' CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
	make -C simplevideo ARCH='$(ARCH)' CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
	make -C simplescene ARCH='$(ARCH)' CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
	make -C simplegui ARCH='$(ARCH)' CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
	make -C simplemodel ARCH='$(ARCH)' CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
	make -C simpleconnect ARCH='$(ARCH)' CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
	make -C simpleconfig ARCH='$(ARCH)' CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
	./scripts/svn2cl.sh | sed 's-  stea-  sgreene-g' > ChangeLog

.PHONY: test
test:	
	make -C simpleutils ARCH='$(ARCH)' CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)'	#No tests!
	make -C simpleaudio ARCH='$(ARCH)' CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)'	#No tests!
	make -C simpletexture ARCH='$(ARCH)' CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
	make -C simplevideo ARCH='$(ARCH)' CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)'	#No tests!
	make -C simplescene ARCH='$(ARCH)' CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)'	#No tests!
	make -C simplegui ARCH='$(ARCH)' CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
	make -C simplemodel ARCH='$(ARCH)' CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
	make -C simpleconnect ARCH='$(ARCH)' CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)'	#No tests!
	make -C simpleconfig ARCH='$(ARCH)' CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@

.PHONY: win32_test
win32_test:	
	make ARCH=$(WARCH) test

.PHONY: test_win32
test_win32:	win32_test

.PHONY: win32_clean
win32_clean:	
	make ARCH=$(WARCH) clean

.PHONY: clean_win32
clean_win32:	win32_clean

.PHONY: clean
clean:
	make -C simpleutils ARCH='$(ARCH)' CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
	make -C simpleaudio ARCH='$(ARCH)' CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
	make -C simpletexture ARCH='$(ARCH)' CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
	make -C simplevideo ARCH='$(ARCH)' CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
	make -C simplescene ARCH='$(ARCH)' CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
	make -C simplegui ARCH='$(ARCH)' CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
	make -C simplemodel ARCH='$(ARCH)' CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
	make -C simpleconnect ARCH='$(ARCH)' CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
	make -C simpleconfig ARCH='$(ARCH)' CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
	rm -f *simple-config ChangeLog

.PHONY: backup
backup:
	make -C simpleutils ARCH='$(ARCH)' CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
	make -C simpleaudio ARCH='$(ARCH)' CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
	make -C simpletexture ARCH='$(ARCH)' CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
	make -C simplevideo ARCH='$(ARCH)' CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
	make -C simplescene ARCH='$(ARCH)' CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
	make -C simplegui ARCH='$(ARCH)' CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
	make -C simplemodel ARCH='$(ARCH)' CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
	make -C simpleconnect ARCH='$(ARCH)' CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
	make -C simpleconfig ARCH='$(ARCH)' CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@

.PHONY: tar
tar:
	make -C simpleutils ARCH='$(ARCH)' CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
	make -C simpleaudio ARCH='$(ARCH)' CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
	make -C simpletexture ARCH='$(ARCH)' CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
	make -C simplevideo ARCH='$(ARCH)' CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
	make -C simplescene ARCH='$(ARCH)' CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
	make -C simplegui ARCH='$(ARCH)' CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
	make -C simplemodel ARCH='$(ARCH)' CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
	make -C simpleconnect ARCH='$(ARCH)' CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
	make -C simpleconfig ARCH='$(ARCH)' CXX='$(CXX)' FLAGS='$(FLAGS)' LIBS='$(LIBS)' $@
