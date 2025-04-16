TARGET_EXEC := crypt

CC := gcc
BUILD_DIR := build
SRC_DIRS := src dep
INC_DIRS := src dep
INC_FLAGS := $(addprefix -I,$(INC_DIRS))
SRCS := $(shell find $(SRC_DIRS) -name '*.c')
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

CFLAGS := -MMD -MP -Wall -Wextra -Werror -Wpedantic -pedantic -pedantic-errors -std=c99 -isystem dep
LDFLAGS := -lpthread

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(INC_FLAGS) $(CFLAGS) -c $< -o $@

run: $(BUILD_DIR)/$(TARGET_EXEC)
	@$(BUILD_DIR)/$(TARGET_EXEC)

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)

-include $(DEPS)
