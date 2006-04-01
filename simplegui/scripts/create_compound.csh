#!/bin/csh -f

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

set name="$argv[1]"
set caps=`echo "$name" | tr a-z A-Z`
set lows=`echo "$name" | tr A-Z a-z`
set hfile="sg_${lows}.h"
set cppfile="sg_${lows}.cpp"

echo "name - $caps - $lows ($cppfile/$hfile)"

cat scripts/sg_basiccompound.h | sed "s-BASICCOMPOUND-$caps-g" \
	| sed "s-BasicCompound-$name-g" > temp_compound.h

cat scripts/sg_basiccompound.cpp | sed "s-basiccompound-$lows-g" \
	| sed "s-BasicCompound-$name-g" > temp_compound.cpp

mv -vi temp_compound.h "$hfile"
mv -vi temp_compound.cpp "$cppfile"
