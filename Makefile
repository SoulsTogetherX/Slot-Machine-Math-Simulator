CXX := g++

SRC_DIR         := src
INC_DIR         := include
THIRD_PARTY_DIR := third_party
BUILD_DIR       := build

CXXFLAGS := -std=c++17 -Wall -Wextra -g -I$(INC_DIR) -isystem $(THIRD_PARTY_DIR)

# The executable suffix depends on the target platform (.exe on Windows).
ifeq ($(OS),Windows_NT)
    TARGET := app.exe
else
    TARGET := app
endif

# The file-manipulation recipes depend on the shell make ACTUALLY uses, not on
# the OS: an MSYS2/Git-Bash/WSL shell provides the Unix coreutils, while a bare
# Windows cmd.exe does not. `uname` runs under that same shell, so a non-empty
# result means we can rely on the Unix commands.
ifeq ($(shell uname 2>NUL),)
    # Native Windows cmd.exe
    RUN_CMD  = $(TARGET)
    MKDIR_P  = if not exist "$(subst /,\,$(1:%/=%))" mkdir "$(subst /,\,$(1:%/=%))"
    RMDIR_RF = if exist "$(subst /,\,$(1:%/=%))" rmdir /s /q "$(subst /,\,$(1:%/=%))"
    RM_F     = if exist "$(subst /,\,$1)" del /q "$(subst /,\,$1)"
else
    # Unix-style shell (Linux, macOS, MSYS2, Git Bash, WSL)
    RUN_CMD  = ./$(TARGET)
    MKDIR_P  = mkdir -p $1
    RMDIR_RF = rm -rf $1
    RM_F     = rm -f $1
endif

rwildcard = $(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

SRCS    := $(call rwildcard,$(SRC_DIR)/,*.cpp)
HEADERS := $(call rwildcard,$(INC_DIR)/,*.hpp)

OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS)
	$(call MKDIR_P,$(dir $@))
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: all
	$(RUN_CMD)

clean:
	$(call RMDIR_RF,$(BUILD_DIR))
	$(call RM_F,$(TARGET))