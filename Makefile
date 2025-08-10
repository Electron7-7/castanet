LINUX_CXX := clang++
LINUX_CC  := clang

ifneq ($(OS),Windows_NT)
	WINDOWS_CXX := x86_64-w64-mingw32-g++
	WINDOWS_CC  := x86_64-w64-mingw32-gcc
else
	WINDOWS_CXX := g++
	WINDOWS_CC  := gcc
endif

FLAGS_DEBUG_COMMON    := -g -Wall -O0 -D DEBUGGING
FLAGS_DEBUG_LINUX     := -fsanitize=address
FLAGS_DEBUG_WINDOWS   := # Nothing yet
FLAGS_RELEASE_COMMON  := -O3
FLAGS_RELEASE_WINDOWS := # Nothing yet
FLAGS_RELEASE_LINUX   := # Nothing yet
FLAGS_CXX_COMMON      := -std=c++20
FLAGS_CC_COMMON       := -std=c11
FLAGS_WINDOWS         := -mwindows -static
FLAGS_LINUX           := # Nothing yet
LDFLAGS_LINUX         := -L src/lib -lgetargs_linux
LDFLAGS_WINDOWS       := -L src/lib -lgetargs_windows

INCLUDE := -I src -I src/include

DIR_ROOT    := build
DIR_LINUX   := Linux
DIR_WINDOWS := Windows
DIR_DEBUG   := Debug
DIR_RELEASE := Release
DIR_OBJS    := .objs

NAME_BASE := castanet

# LINUX
ifneq ($(OS),Windows_NT)
	export NAME          ?= $(NAME_BASE)
	export BUILD_ARCH    ?= $(DIR_LINUX)
	export DEBUG_FLAGS   ?= $(FLAGS_DEBUG_COMMON) $(FLAGS_DEBUG_LINUX)
	export RELEASE_FLAGS ?= $(FLAGS_RELEASE_COMMON) $(FLAGS_RELEASE_LINUX)
	export CXX_FLAGS     ?= $(FLAGS_CXX_COMMON) $(FLAGS_LINUX)
	export CC_FLAGS      ?= $(FLAGS_CC_COMMON) $(FLAGS_LINUX)
	export LD_FLAGS      ?= $(LDFLAGS_LINUX)
	export CXX_COMPILER  ?= $(LINUX_CXX)
	export C_COMPILER    ?= $(LINUX_CC)
else # WINDOWS
	export NAME          ?= $(NAME_BASE).exe
	export BUILD_ARCH    ?= $(DIR_WINDOWS)
	export DEBUG_FLAGS   ?= $(FLAGS_DEBUG_COMMON) $(FLAGS_DEBUG_WINDOWS)
	export RELEASE_FLAGS ?= $(FLAGS_RELEASE_COMMON) $(FLAGS_RELEASE_WINDOWS)
	export CXX_FLAGS     ?= $(FLAGS_CXX_COMMON) $(FLAGS_WINDOWS)
	export CC_FLAGS      ?= $(FLAGS_CC_COMMON) $(FLAGS_WINDOWS)
	export LD_FLAGS      ?= $(LDFLAGS_WINDOWS)
	export CXX_COMPILER  ?= $(WINDOWS_CXX)
	export C_COMPILER    ?= $(WINDOWS_CC)
endif

export BUILD_VERSION ?= $(DIR_RELEASE)
export VERSION_FLAGS ?= $(RELEASE_FLAGS)

export BUILD_DIR  ?= $(DIR_ROOT)/$(BUILD_ARCH)/$(BUILD_VERSION)
export BUILD_OBJS ?= $(BUILD_DIR)/$(DIR_OBJS)

VPATH := $(SRC_DIRS)

SRC := src

SRC_DIRS :=          \
    $(SRC)/arguments \
    $(SRC)/system    \


CC_SRCS  := $(foreach directory,$(SRC_DIRS),$(wildcard $(directory)/*.c))
CXX_SRCS := $(foreach directory,$(SRC_DIRS),$(wildcard $(directory)/*.cpp))

export CC_OBJS  ?= $(addprefix $(BUILD_OBJS)/,$(subst .c,.o,$(CC_SRCS:$(SRC)/%=%)))
export CXX_OBJS ?= $(addprefix $(BUILD_OBJS)/,$(subst .cpp,.obj,$(CXX_SRCS:$(SRC)/%=%)))

export RESET   ?= \\x1b[0m
export BLACK   ?= \\x1b[1;30m
export RED     ?= \\x1b[1;31m
export GREEN   ?= \\x1b[1;32m
export YELLOW  ?= \\x1b[1;33m
export BLUE    ?= \\x1b[1;34m
export MAGENTA ?= \\x1b[1;35m
export CYAN    ?= \\x1b[1;36m
export WHITE   ?= \\x1b[1;37m
export DEFAULT ?= \\x1b[1;39m

.PHONY: build linux windows release debug build_dir clean disable_colors

build:
	@ printf "$(DEFAULT)::Architecture - $(BLUE)$(BUILD_ARCH)$(RESET)\n"
	@ printf "$(DEFAULT)::Version - $(BLUE)$(BUILD_VERSION)$(RESET)\n"
	@ printf "$(DEFAULT)::C Compile Command - $(YELLOW)$(C_COMPILER) $(CC_FLAGS) $(VERSION_FLAGS) $(INCLUDE)$(RESET)\n"
	@ printf "$(DEFAULT)::C++ Compile Command - $(YELLOW)$(CXX_COMPILER) $(CXX_FLAGS) $(VERSION_FLAGS) $(INCLUDE)$(RESET)\n"
	@ $(MAKE) -s $(BUILD_DIR)/$(NAME)
	@ printf "$(DEFAULT)::Program Location - $(GREEN)$(DIR_ROOT)/$(BUILD_ARCH)/$(BUILD_VERSION)/$(NAME)$(RESET)\n"

linux: ;@:
	$(eval NAME          = $(NAME_BASE))
	$(eval BUILD_ARCH    = $(DIR_LINUX))
	$(eval DEBUG_FLAGS   = $(FLAGS_DEBUG_COMMON) $(FLAGS_DEBUG_LINUX))
	$(eval RELEASE_FLAGS = $(FLAGS_RELEASE_COMMON) $(FLAGS_RELEASE_LINUX))
	$(eval CXX_FLAGS     = $(FLAGS_CXX_COMMON) $(FLAGS_LINUX))
	$(eval CC_FLAGS      = $(FLAGS_CC_COMMON) $(FLAGS_LINUX))
	$(eval LD_FLAGS      = $(LDFLAGS_LINUX))
	$(eval CXX_COMPILER  = $(LINUX_CXX))
	$(eval C_COMPILER    = $(LINUX_CC))

windows: ;@:
	$(eval NAME          = $(NAME_BASE).exe)
	$(eval BUILD_ARCH    = $(DIR_WINDOWS))
	$(eval DEBUG_FLAGS   = $(FLAGS_DEBUG_COMMON) $(FLAGS_DEBUG_WINDOWS))
	$(eval RELEASE_FLAGS = $(FLAGS_RELEASE_COMMON) $(FLAGS_RELEASE_WINDOWS))
	$(eval CXX_FLAGS     = $(FLAGS_CXX_COMMON) $(FLAGS_WINDOWS))
	$(eval CC_FLAGS      = $(FLAGS_CC_COMMON) $(FLAGS_WINDOWS))
	$(eval LD_FLAGS      = $(LDFLAGS_WINDOWS))
	$(eval CXX_COMPILER  = $(WINDOWS_CXX))
	$(eval C_COMPILER    = $(WINDOWS_CC))

release: ;@:
	$(eval VERSION_FLAGS = $(RELEASE_FLAGS))
	$(eval BUILD_VERSION = $(DIR_RELEASE))

debug: ;@:
	$(eval VERSION_FLAGS = $(DEBUG_FLAGS))
	$(eval BUILD_VERSION = $(DIR_DEBUG))

build_dir:
	@ -mkdir -p $(BUILD_DIR) $(BUILD_OBJS)

clean:
	@ -rm -rf $(DIR_ROOT)
	@ printf "::Cleaned $(RED)$(DIR_ROOT)/$(RESET)\n"

disable_colors:
	$(eval RESET   := "")
	$(eval BLACK   := "")
	$(eval RED     := "")
	$(eval GREEN   := "")
	$(eval YELLOW  := "")
	$(eval BLUE    := "")
	$(eval MAGENTA := "")
	$(eval CYAN    := "")
	$(eval WHITE   := "")
	$(eval DEFAULT := "")
	@ printf "::Output colors disabled\n"

$(BUILD_OBJS)/%.obj: $(SRC)/%.cpp | build_dir
	@ printf "::Compiling $(BLUE)$@$(RESET)\n"
	@ -mkdir -p $(dir $@)
	$(CXX_COMPILER) $(CXX_FLAGS) $(VERSION_FLAGS) $(INCLUDE) -c $< -o $@

$(BUILD_OBJS)/%.o: $(SRC)/%.c | build_dir
	@ printf "::Compiling $(BLUE)$@$(RESET)\n"
	@ -mkdir -p $(dir $@)
	$(C_COMPILER) $(CC_FLAGS) $(VERSION_FLAGS) $(INCLUDE) -c $< -o $@

$(BUILD_DIR)/$(NAME): $(CC_OBJS) $(CXX_OBJS)
	@ printf "::Linking $(CYAN)$@$(RESET)\n"
	$(CXX_COMPILER) $(CXX_FLAGS) $(VERSION_FLAGS) $(INCLUDE) $^ -o $@ $(LD_FLAGS)

