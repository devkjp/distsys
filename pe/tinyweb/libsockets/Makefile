#=============================================================================
#
# Makefile
#
#-----------------------------------------------------------------------------
#
# DHBW Ravensburg - Campus Friedrichshafen
#
# Vorlesung Systemnahe Programmierung / Verteilte Systeme
#
#-----------------------------------------------------------------------------
#
# Author: Ralf Reutemann
#
# $Id: Makefile,v 1.3 2014/06/11 14:39:22 ralf Exp ralf $
#
#=============================================================================

CC = gcc
CFLAGS = -g -O2 -Wall -pedantic -fPIC

#-----------------------------------------------------------------------------
# Configure OS/Architecture-specific build directory and create if necessary
#-----------------------------------------------------------------------------
OS          := $(shell uname -s)
ARCH        := $(shell uname -m)
SRC_DIR     := .
BUILD_DIR   := build/$(OS)_$(ARCH)
OBJ_DIR     := $(BUILD_DIR)
dummy1      := $(shell test -d $(BUILD_DIR) || mkdir -p $(BUILD_DIR))
#-----------------------------------------------------------------------------

SRCS        := $(wildcard $(SRC_DIR)/*.c)
OBJS        := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

TARGETS = $(BUILD_DIR)/libsockets.a $(BUILD_DIR)/libsockets.so

.phony:
all: $(TARGETS)

$(BUILD_DIR)/libsockets.a : $(OBJS)
	rm -f $@
	ar qcv $@ $(OBJS)
	ranlib $@

$(BUILD_DIR)/libsockets.so : $(OBJS)
	$(CC) -shared -o $@ $(OBJS)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	@echo CC $<
	@$(CC) $(CFLAGS) -I$(SRC_DIR) -o $(OBJ_DIR)/$*.o -c $<

.phony:
depend:
	makedepend $(SRC)

.phony:
clean:
	rm -f $(BUILD_DIR)/*.o $(TARGETS)

