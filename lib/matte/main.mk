#
# lib/matte/main.mk
#

RCD := lib/matte
ENVIRONMENT := lib
include $(TOPD)/env.mk

target := matte.a
csrc   := init.c text.c

include $(TOPD)/build/template/ar.mk
