# *************************************************************************
#  This file is part of the Simple* Example Suite by Steaphan Greene
#
#  Copyright 2005-2006 Steaphan Greene <stea@cs.binghamton.edu>
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

INSTALL = /usr/bin/install

all:	build

build:
	make -C simpleaudio
	make -C simpletexture
	make -C simplevideo
	make -C simplegui
	make -C simplemodel
	make -C simpleconnect
	make -C simpleconfig

install:	all
	$(INSTALL) -d $(LIBDIR)
	$(INSTALL) -d $(INCDIR)
	$(INSTALL) -m 644 simple*/lib*.a $(LIBDIR)
	$(INSTALL) -m 644 simple*/*.h $(INCDIR)

uninstall:	all
	rm -f $(LIBDIR)/libsimple*.a
	rm -f $(INCDIR)/s*.h

win32:
	make -C simpleaudio $@
	make -C simpletexture $@
	make -C simplevideo $@
	make -C simplegui $@
	make -C simplemodel $@
	make -C simpleconnect $@
	make -C simpleconfig $@

ChangeLog:
	make -C simpleaudio $@
	make -C simpletexture $@
	make -C simplevideo $@
	make -C simplegui $@
	make -C simplemodel $@
	make -C simpleconnect $@
	make -C simpleconfig $@

test:	
	#make -C simpleaudio $@
	make -C simpletexture $@
	#make -C simplevideo $@
	make -C simplegui $@
	make -C simplemodel $@
	#make -C simpleconnect $@
	make -C simpleconfig $@

win32_test:	
	make -C simpleaudio $@
	make -C simpletexture $@
	make -C simplevideo $@
	make -C simplegui $@
	make -C simplemodel $@
	make -C simpleconnect $@
	make -C simpleconfig $@

wintest:
	make -C simpleaudio $@
	make -C simpletexture $@
	make -C simplevideo $@
	make -C simplegui $@
	make -C simplemodel $@
	make -C simpleconnect $@
	make -C simpleconfig $@

clean:
	make -C simpleaudio $@
	make -C simpletexture $@
	make -C simplevideo $@
	make -C simplegui $@
	make -C simplemodel $@
	make -C simpleconnect $@
	make -C simpleconfig $@

backup:
	make -C simpleaudio $@
	make -C simpletexture $@
	make -C simplevideo $@
	make -C simplegui $@
	make -C simplemodel $@
	make -C simpleconnect $@
	make -C simpleconfig $@

tar:
	make -C simpleaudio $@
	make -C simpletexture $@
	make -C simplevideo $@
	make -C simplegui $@
	make -C simplemodel $@
	make -C simpleconnect $@
	make -C simpleconfig $@
