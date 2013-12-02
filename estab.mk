#
# estab.mk
#

SHELL := /bin/sh

.PHONY: all clean

# standard build utilities
CC  := gcc -c
LD  := gcc
AS  := nasm
AR  := ar
FSB := ./util/fsb/fsb

# compilation flags
ERR_FL   = -Wall -Wextra -Wuninitialized -Wno-unused-parameter

TOP_CCFL = $(ERR_FL) -I$(TOPD)/include/$(ENVIRONMENT)/ -DPLATFORM=$(PLATFORM)
TOP_LDFL = $(ERR_FL)
TOP_ASFL = -Wall -I$(TOPD)/include/$(ENVIRONMENT)/ -I$(CD)/src/ -DPLATFORM=$(PLATFORM) -felf64

CCFL = $(TOP_CCFL) $(ENV_CCFL) $(PF_CCFL) $(ccxfl)
LDFL = $(TOP_LDFL) $(ENV_LDFL) $(PF_LDFL) $(ldxfl)
ASFL = $(TOP_ASFL) $(ENV_ASFL) $(PF_ASFL) $(asxfl)

# Directories
TOPD := $(realpath $(dir $(lastword $(MAKEFILE_LIST))))
CD = $(TOPD)/$(RCD)

# verbosity
ifndef V
  V := 1
endif

ifeq ($V,3)
  override V := 2
else
  QUIET := @
endif

CMDL := $(TOPD)/build/verbose/ver$V.sh
CMD  := @$(CMDL)

FILE  = $(if $(wildcard $(PLATFORM)/$1), $(PLATFORM)/$1, $1)
FILES = $(foreach file, $1, $(call $(FILE),$(file)))
