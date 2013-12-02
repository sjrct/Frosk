#
# boot/main.mk
#

RCD := boot
ENVIRONMENT := boot

include $(TOPD)/env.mk

target = boot.bin
src    = boot.asm
dep    = leftovers.inc before/read.inc before/error16.inc after/a20.inc

src := $(addprefix $(CD)/src/,$(src))
dep := $(addprefix $(CD)/src/,$(dep))

define scope
$(CD)/$(target): $(src) $(dep)
	$(CMD) "$(AS) $(ASFL) -fbin -o $$@ $(src)" "AS $$(subst $(CD)/,$(RCD)/,$$@)"

$(CD)/clean:
	-$(CMD) "$(RM) -f $(CD)/$(target)" "RM $(RCD)/$(target)"
endef

$(eval $(scope))
