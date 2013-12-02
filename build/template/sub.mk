#
# build/template/main.mk
#

submake  := $(foreach u,$(sub),$(RCD)/$u/$u$(smsuf))
subclean := $(foreach u,$(sub),$(RCD)/$u/clean)
subinc   := $(foreach u,$(sub),$(CD)/$u/main.mk)

define scope
.PHONY: $(subclean)

$(RCD)/$(target): $(submake)
	$(QUIET) touch $(CD)/$(target)

$(RCD)/clean: $(subclean)
	-$(QUIET) $(RM) $(CD)/$(target)
endef

$(eval $(scope))

include $(subinc)
