#
# build/template/general.mk
#

obj := $(addprefix $(CD)/obj/,$(csrc:.c=.c.o) $(asrc:.asm=.asm.o))
dep := $(addprefix $(CD)/dep/,$(csrc:.c=.c.D) $(asrc:.asm=.asm.D))

define scope
$(RCD)/$(target): $(obj) $(lib)
	$(link_string)

$(CD)/obj/%.c.o: $(CD)/src/%.c
	$(QUIET) mkdir $$(@D) -p
	$(QUIET) mkdir $$(subst obj,dep,$$(@D)) -p
	$(QUIET) $(CC) $(CCFL) -M -MP -MF $(CD)/dep/$$*.c.D -MT $$@ $$<
	$(CMD) "$(CC) -g $(CCFL) -o $$@ $$<" "CC $$(subst $(CD)/,$(RCD)/,$$@)"

$(CD)/obj/%.asm.o: $(CD)/src/%.asm
	$(QUIET) mkdir $$(@D) -p
	$(QUIET) mkdir $$(subst obj,dep,$$(@D)) -p
	$(QUIET) $(AS) $(ASFL) -M -MP -MT $$@ $$< > $(CD)/dep/$$*.asm.D
	$(CMD) "$(AS) -g $(ASFL) -o $$@ $$<" "AS $$(subst $(CD)/,$(RCD)/,$$@)"

$(RCD)/clean:
	-$(CMD) "$(RM) -f $(CD)/$(target)" "RM $(RCD)/$(target)"
	-$(CMD) "$(RM) -rf $(CD)/obj" "RM $(RCD)/obj/"
	-$(CMD) "$(RM) -rf $(CD)/dep" "RM $(RCD)/dep/"
endef

$(eval $(scope))

-include $(dep)
