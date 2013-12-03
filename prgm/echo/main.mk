#
# prgm/echo/main.mk
#

RCD := prgm/echo
ENVIRONMENT := prgm
include $(TOPD)/env.mk

target := echo
csrc   := main.c
lib    := frosk

include $(TOPD)/build/template/ld.mk
