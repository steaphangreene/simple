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

module=`pwd | tr '/' '\n' | grep -v '^$' | tail -1`
make ChangeLog
scp ChangeLog optimus:public_html/${module}/ChangeLog.txt

if test $module = simple ; then
  for submod in simple[a-z]* ; do
    scp ${submod}/ChangeLog optimus:public_html/${submod}/ChangeLog.txt
  done
fi
