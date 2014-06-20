#! /usr/bin/python
# colorcov.py
# by chyyuu

import sys
import os
import time
from termcolor import colored

lineset=[]
linenumfile=open('allcovnum.log','r')
for line in linenumfile:
	v=int(line)
#	print v
	lineset.append(v)
lineset.sort()
#print lineset
progfile=open('calculator.c','r')
v=0
for line in progfile:
	v+=1
	line=line.rstrip()	
	if  v in lineset :
		print colored(str(v)+":", 'yellow'), colored(line, 'green')
 	else: 
		print colored(str(v)+":", 'yellow'), colored(line, 'red')

