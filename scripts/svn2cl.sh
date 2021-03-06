#!/bin/sh

# *************************************************************************
#  This file is part of the Simple* Example Suite by Steaphan Greene
#
#  Copyright 2005-2015 Steaphan Greene <steaphan@gmail.com>
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

svn log -r HEAD:1 | sed 's-(no author)-Unknown-g' | sed 's-^r[0-9]* | \([A-Za-z0-9]*\) | \([0-9-]* [0-9]*:[0-9]*\):.*$-|\2  \1|-g' | grep -v '^$' | grep -v -- ------------------------------------------------------------------------ | sed 's-^\([^|]\)-\t* \1-g' | tr '|' '\n'
