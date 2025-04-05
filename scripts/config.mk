# Copyright (c) 2025 HUSTJunshuang
# 
# Licensed under the MIT License. See LICENSE file in the project root for full license information.

COLOR_RED := $(shell echo "\033[1;31m")
COLOR_END := $(shell echo "\033[0m")

# check existense of '.config' file
ifeq ($(wildcard .config),)
$(warning $(COLOR_RED)Warning: .config does not exists!$(COLOR_END))
$(warning $(COLOR_RED)To build the project, run 'make menuconfig' first.$(COLOR_END))
endif

Q := @
rm-distclean += include/generated include/config .config .config.old
silent := -s

CONF	:= kconfig-conf
MCONF 	:= kconfig-mconf
PY 		:= python

menuconfig:
	$(Q)$(MCONF) Kconfig
	$(Q)$(PY) scripts/kconfig.py

distclean: clean
	-@rm -rf $(rm-distclean)

.PHONY: menuconfig distclean
