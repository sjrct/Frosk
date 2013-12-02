#
# kernel/main.mk
#

RCD := kernel
ENVIRONMENT := kernel
include $(TOPD)/env.mk

target := kernel.bin
csrc   := main.c \
          exec.c \
          error.c \
          paging.c \
          wrappers.c \
          interrupt.c \
          scheduler.c \
          handle_set.c \
          \
          io/io.c \
          io/file.c \
          io/kprint.c \
          io/net/serial.c \
          dev/dev.c \
          dev/ps2/ps2.c \
          dev/disk/ata.c \
          dev/video/text/vga.c \
          \
          cstd/string.c \
          extra/string.c \
          \
          memory/pages.c \
          memory/kernel.c \
          memory/region.c

asrc   := tss.asm \
          lock.asm \
          paging.asm \
          syscall.asm \
          interrupt.asm \
          scheduler.asm \
          exceptions.asm \
          dev/dev.asm

# The lack of -ftree-ter causes problems sometimes in qemu
# Ideally the necessity for this should be removed

ccxfl := -m64 -ffreestanding -nostdinc -mcmodel=large -mno-red-zone -DDEBUG
asxfl :=
ldxfl := -m64 -ffreestanding -nostdlib -mcmodel=large -mno-red-zone -T$(CD)/kernel.ld

include $(TOPD)/build/template/ld.mk
