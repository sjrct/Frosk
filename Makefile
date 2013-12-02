#
# Makefile
#

include estab.mk

target   := frosk.img
sub      := boot kernel util lib prgm
subext   := .bin .bin .stb .stb .stb
submake  := $(join $(foreach X,$(sub),$X/$X),$(subext))
subclean := $(foreach X,$(sub),$X/clean)
subinc   := $(foreach X,$(sub),$X/main.mk)

.PHONY: $(subclean)

define scope
all: $(target)

$(target): $(submake)
	$(CMD) "$(FSB) -i default.fsd -o $(target)" "FSB $(target)"
	$(CMD) "dd if=boot/boot.bin of=$(target) conv=notrunc" "DD $(target)" 2> /dev/null

clean: $(subclean)
	-$(CMD) "$(RM) $(target)" "RM $(target)"
endef

$(eval $(scope))

# shortcuts
kernel: kernel/kernel.bin
boot:   boot/boot.bin
frash:  prgm/frash/frash

include $(subinc)
