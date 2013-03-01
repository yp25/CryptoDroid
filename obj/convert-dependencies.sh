#!/bin/sh
# AUTO-GENERATED FILE, DO NOT EDIT!
if [ -f $1.org ]; then
  sed -e 's!^C:/cygwin/lib!/usr/lib!ig;s! C:/cygwin/lib! /usr/lib!ig;s!^C:/cygwin/bin!/usr/bin!ig;s! C:/cygwin/bin! /usr/bin!ig;s!^C:/cygwin/!/!ig;s! C:/cygwin/! /!ig;s!^Z:!/cygdrive/z!ig;s! Z:! /cygdrive/z!ig;s!^S:!/cygdrive/s!ig;s! S:! /cygdrive/s!ig;s!^R:!/cygdrive/r!ig;s! R:! /cygdrive/r!ig;s!^P:!/cygdrive/p!ig;s! P:! /cygdrive/p!ig;s!^H:!/cygdrive/h!ig;s! H:! /cygdrive/h!ig;s!^C:!/cygdrive/c!ig;s! C:! /cygdrive/c!ig;' $1.org > $1 && rm -f $1.org
fi
