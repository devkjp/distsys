#**********************************************************************
# Berufsakademie Ravensburg - Praktikum Verteilte Kommunikationssysteme
#
# $Id: Makefile,v 1.1 2001/05/09 23:16:47 reuteman Exp reuteman $
#
#**********************************************************************


SHELL		= /bin/sh

CC		= gcc
CFLAGS		= -Wall -g -O2 -pedantic
PS2PDF		= ps2pdf
A2PS		= a2ps
AOPT		= --line-numbers=1

OBJECTS = echod.o passive_tcp.o socket_io.o

.PHONY : all
all : echod

%.pdf : %.ps
	$(PS2PDF) $< $@

%.ps : %.c
	$(A2PS) $< $(AOPT) -o $@

echod : $(OBJECTS)
	$(CC) -o $@ $(OBJECTS) -lpthread

.PHONY : clean
clean :
	rm -r echod
