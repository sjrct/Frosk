#
# prgm/main.mk
#

RCD := prgm
ENVIRONMENT := prgm
include $(TOPD)/env.mk

target := prgm.stb
sub    := frash echo
smsuf  :=

include $(TOPD)/build/template/sub.mk
