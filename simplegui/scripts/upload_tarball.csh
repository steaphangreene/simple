#!/bin/csh -f

make tar
scp simplegui.*.tar.gz opal:public_html/simplegui/tarballs/
rm -vf simplegui.*.tar.gz
