#
# util/main.mk
#

RCD := util
ENVIRONMENT := util
include $(TOPD)/env.mk

target := util.stb
sub    := fsb
smsuf  :=

include $(TOPD)/build/template/sub.mk
