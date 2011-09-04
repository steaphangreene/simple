#!/bin/sh

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

prefix="/usr/local"
exec_prefix="${prefix}"
package="simple"
version="0.0.0-svnREVISION"
includedir="${prefix}/include/simple"
cross_prefix=""
arch="`gcc -v 2>&1 | grep Target | cut -f2 -d' '`"
libdir="${exec_prefix}/lib/simple/${arch}"
base_libs="-lSDL_net -lSDL_ttf -lSDL_image -lSDL_mixer"
extra_libs=""
gl_libs="-lGL -lGLU"

if [ $arch == i586-mingw32msvc ]; then
  cross_prefix="/opt/i586-mingw32msvc/bin/"
  extra_libs="-lSDL -lpng -ltiff -ljpeg -lz"
  gl_libs="-lopengl32 -lglu32"
fi

if test $# -eq 0; then
    cat <<EOF
Usage:   $package-config [OPTIONS]
Options:
    --prefix[=DIR]) : \$prefix
    --package) : \$package
    --version) : \$version
    --cflags) : <long>
    --libs) : <long>
EOF
fi

output=""
for i in "$@" ; do
    case $i in
    --prefix=*) $prefix=`echo $prefix | sed -e 's/--prefix=//'` ;;
    --prefix)   output="$output $prefix" ;;
    --package)  output="$output $package" ;;
    --version)  output="$output $version" ;;
    --cflags)   output="$output `${cross_prefix}pkg-config sdl --cflags` `${cross_prefix}pkg-config zziplib --cflags` -I$includedir" ;;
    --libs) output="$output -L$libdir -lsimpleconnect -lsimpleconfig -lsimpleaudio -lsimplevideo -lsimplescene -lsimplegui -lsimplemodel -lsimpletexture -lsimpleutils `${cross_prefix}pkg-config sdl --libs` ${base_libs} `${cross_prefix}pkg-config zziplib --libs` ${extra_libs} ${gl_libs}"
        ;;
    *) output="$output $i" ;;
    esac
done
echo $output

