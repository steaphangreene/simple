#!/bin/csh -f

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
