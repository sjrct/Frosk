#
# build/template/ar.mk
#

link_string := $(CMD) "$(AR) rcu $(CD)/$(target) $$^" "AR $(RCD)/$(target)"
include $(TOPD)/build/template/general.mk
