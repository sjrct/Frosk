#
# build/template/ld.mk
#

lib := $(foreach l,$(lib),$(TOPD)/lib/$l/$l.a)

link_string = $(CMD) "$(LD) $(LDFL) -o $(CD)/$(target) $$^ $(lib)" "LD $(RCD)/$(target)"
include $(TOPD)/build/template/general.mk
