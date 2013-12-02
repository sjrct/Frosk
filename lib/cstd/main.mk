#
# lib/cstd/main.mk
#

RCD := lib/cstd
ENVIRONMENT := lib
include $(TOPD)/env.mk

target := cstd.a
csrc   := string.c ctype.c rand.c malloc.c

include $(TOPD)/build/template/ar.mk
