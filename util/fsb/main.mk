#
# util/fsb/main.mk
#

RCD := util/fsb
ENVIRONMENT := util
include $(TOPD)/env.mk

target := fsb
csrc   := main.c fs.c parse.c directive.c

include $(TOPD)/build/template/ld.mk
