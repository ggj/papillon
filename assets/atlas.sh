#!/bin/bash
./texpack -l filelist.txt -b 0 -o papillon -H 1024 -w 1024 -n -f output.txt --format="<resource type='image' filename='%s' name='%s' x='%u' y='%u' width='%u' height='%u' />" -d .
#./texpack -l hud.txt -b 2 -o hud -H 1024 -w 1024 -n -f output-hud.txt --format="<resource type='image' filename='%s' name='%s' x='%u' y='%u' width='%u' height='%u' />" -d .
