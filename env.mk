#
# env.mk
#

ifndef PLATFORM
  PLATFORM = x86_64
endif

ccxfl  :=
ldxfl  :=
asxfl  :=
target :=
csrc   :=
asrc   :=

ifdef ENVIRONMENT
  -include $(TOPD)/build/make/$(ENVIRONMENT).mk
  -include $(TOPD)/build/make/$(PLATFORM)/$(ENVIRONMENT).mk
endif

