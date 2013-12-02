#
# lib/extra/main.mk
#

RCD := lib/extra
ENVIRONMENT := lib
include $(TOPD)/env.mk

target := extra.a
csrc   := string.c

include $(TOPD)/build/template/ar.mk
