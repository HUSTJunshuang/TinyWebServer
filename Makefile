# Copyright (c) 2025 HUSTJunshuang
# 
# Licensed under the MIT License. See LICENSE file in the project root for full license information.

# compilers
CXX = g++

# paths
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj

# compilation flags
INC_PATH = include/ 3rd/googletest/googletest/include/ 3rd/googletest/googlemock/include/
INCFLAGS += $(addprefix -I, $(INC_PATH))
LIB_PATH = 3rd/googletest/lib/ 3rd/googlemock/lib/
LDFLAGS += $(addprefix -L, $(LIB_PATH))
LIBS ?=
CXXFLAGS += -std=c++17 -MMD -MP -Wall -Wextra $(INCFLAGS)

# targets
TEST_SRCS = $(shell find test/ -type f -name '*.cpp')
TEST_BINS = $(addprefix $(BUILD_DIR)/, $(basename $(TEST_SRCS)))
DEPS += $(TEST_SRCS:%.cpp=$(BUILD_DIR)/%.d)

$(info $(TEST_SRCS))
$(info $(TEST_BINS))

-include $(DEPS)

$(TEST_BINS): LIBS += -lgtest
$(TEST_BINS): $(TEST_SRCS)
$(BUILD_DIR)/test/%: test/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $< $(LIBS) -o $@

test: $(TEST_BINS)

clean:
	-@rm -rf $(BUILD_DIR)

# include variables generated by menuconfig
-include include/config/auto.conf

# include rules for menuconfig
include scripts/config.mk