#!/bin/csh -f

# *************************************************************************
#  This file is part of the SimpleGUI Example Module by Steaphan Greene
#
#  Copyright 2005 Steaphan Greene <stea@cs.binghamton.edu>
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

if("$#argv" != "1") then
  echo "USAGE: $0 <basefilename>"
  exit
endif

if(!(-r "${1}.raw")) then
  echo "${1}.raw can not be read."
  exit
endif

set fl=`echo "$0" | sed 's-raw2header.csh-hexdump_format-g'`

echo "WARNING! DOESN'T WORK IF SIZE OF RAW IS NOT DIVISIBLE BY 8!"

echo "static unsigned char $1[] = {" > "${1}.h"
hexdump -vf "$fl" "${1}.raw" >> "${1}.h"
echo "~};" | tr '~' '\t' >> "${1}.h"
