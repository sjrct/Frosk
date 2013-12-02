#
# build/make/prgm.mk
#

ENV_CCFL := -m64 -ffreestanding -nostdinc -mcmodel=large -mno-red-zone
ENV_ASFL :=
ENV_LDFL := -m64 -ffreestanding -nostdlib -mcmodel=large -mno-red-zone -T$(TOPD)/prgm/prgm.ld
