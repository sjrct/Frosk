#
# prgm/frash/main.mk
#

RCD := prgm/frash
ENVIRONMENT := prgm
include $(TOPD)/env.mk

target := frash
csrc   := main.c
lib    := matte cstd frosk

include $(TOPD)/build/template/ld.mk
