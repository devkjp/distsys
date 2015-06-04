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
#=============================================================================

CC          = gcc
LD          = ld
NASM        = nasm
NASMOPT     = -g -f elf -F dwarf
CFLAGS      = -Wall -Werror -pedantic -std=gnu99
CFLAGS     += -Wunreachable-code -Wswitch-default
CFLAGS     += -g -O2
PS2PDF      = ps2pdf
A2PS        = a2ps
AOPT        = --line-numbers=1


SRC_DIR     := src

#-----------------------------------------------------------------------------
# Configure OS/Architecture-specific build directory and create if necessary
#-----------------------------------------------------------------------------
OS          := $(shell uname -s)
ARCH        := $(shell uname -m)
BUILD_DIR   := build/$(OS)_$(ARCH)
OBJ_DIR     := $(BUILD_DIR)/obj
dummy1      := $(shell test -d $(BUILD_DIR) || mkdir -p $(BUILD_DIR))
dummy2      := $(shell test -d $(OBJ_DIR) || mkdir -p $(OBJ_DIR))
#-----------------------------------------------------------------------------

SRCS        := $(wildcard $(SRC_DIR)/*.c)
OBJS        := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
LIBS        := libsockets/$(BUILD_DIR)/libsockets.a

TARGETS = $(BUILD_DIR)/tinyweb


.PHONY: all
all: $(TARGETS)

$(BUILD_DIR)/tinyweb : $(OBJS) $(LIBS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

$(LIBS):
	$(MAKE) -C libsockets

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	@echo CC $<
	@$(CC) $(CFLAGS) -I$(SRC_DIR) -Ilibsockets -o $(OBJ_DIR)/$*.o -c $<

%.pdf : %.ps
	$(PS2PDF) $< $@

.PHONY: clean
clean:
	rm -f *.ps $(TARGETS)
	rm -rf $(BUILD_DIR)

.PHONY: depend
depend:
	makedepend $(SRCS)

# DO NOT DELETE
