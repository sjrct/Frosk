#
# prgm/frash/main.mk
#

RCD := prgm/frash
ENVIRONMENT := prgm
include $(TOPD)/env.mk

target := frash
csrc   := main.c exec.c parse.c
lib    := matte cstd frosk

include $(TOPD)/build/template/ld.mk
