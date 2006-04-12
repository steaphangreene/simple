#!/bin/sh

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

prefix="/usr/local"
exec_prefix="${prefix}"
package="simple"
version="0.12.83"
includedir="${prefix}/include/simple"
libdir="${exec_prefix}/lib/simple"

if test $# -eq 0; then
    cat <<EOF
Usage:   $package-config [OPTIONS]
Options:
    --prefix[=DIR]) : \$prefix
    --package) : \$package
    --version) : \$version
    --cflags) : -I\$includedir
    --libs) : -L\$libdir -lzzip -lz
EOF
fi

output=""
for i in "$@" ; do
    case $i in
    --prefix=*) $prefix=`echo $prefix | sed -e 's/--prefix=//'` ;;
    --prefix)   output="$output $prefix" ;;
    --package)  output="$output $package" ;;
    --version)  output="$output $version" ;;
    --cflags)   output="$output `sdl-config --cflags` `zzip-config --cflags` -I$includedir" ;;
    --libs) output="$output `sdl-config --libs` `zzip-config --libs` -L$libdir -lsimpleconnect -lsimpleconfig -lsimpleaudio -lsimplevideo -lsimplegui -lsimplemodel -lsimpletexture -lSDL_net -lSDL_ttf -lSDL_image -lSDL_mixer `zzip-config --libs` -lGL -lGLU"
        ;;
    *) output="$output $i" ;;
    esac
done
echo $output

