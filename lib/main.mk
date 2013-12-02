#
# lib/main.mk
#

RCD := lib
ENVIRONMENT := lib
include $(TOPD)/env.mk

target := lib.stb
sub    := frosk cstd extra matte
smsuf  := .a

include $(TOPD)/build/template/sub.mk
