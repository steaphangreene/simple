#!/bin/csh -f

scp test1.exe opal:public_html/simplegui/win32/
ssh opal 'cd public_html/simplegui/win32/; make'
