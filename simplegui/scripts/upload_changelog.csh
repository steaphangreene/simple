#!/bin/csh -f

make ChangeLog
scp ChangeLog opal:public_html/simplegui/ChangeLog.txt
