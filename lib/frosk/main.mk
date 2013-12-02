#
# lib/frosk/main.mk
#

RCD := lib/frosk
ENVIRONMENT := lib
include $(TOPD)/env.mk

target := frosk.a
csrc   :=
asrc   := syscall.asm

include $(TOPD)/build/template/ar.mk
