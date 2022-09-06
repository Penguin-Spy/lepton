SRCDIR   = src
LIBDIR   = lib
BUILDDIR = build
OBJDIR   = $(BUILDDIR)/obj
BINDIR   = $(BUILDDIR)/bin

OBJFILES = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.obj,$(wildcard $(SRCDIR)/*.c)) $(patsubst $(SRCDIR)/%.S,$(OBJDIR)/%.obj,$(wildcard $(SRCDIR)/*.S))
LIBFILES = $(wildcard $(SRCDIR)/*.a)

C_INCLUDES = -I$(SRCDIR)/inc -I$(SRCDIR)
C_FLAGS   = $(ARCH) -O4

# Cross-platform directory stuff
ifeq ($(OS),Windows_NT)
RM         = del /q /f $(NOSTDOUT) $(NOSTDERR)
RMDIR      = if exist "$1" rmdir /s /q "$1"
MKDIR      = if not exist "$1" mkdir "$1"
else
RM         = rm -f
RMDIR      = rm -rf $1
MKDIR      = mkdir -p $1
endif
ENSUREDIR  = $(call MKDIR,$(dir $@))

CC = gcc

all: $(BINDIR)/lepton.exe

# Build C object
$(OBJDIR)/%.obj: $(SRCDIR)/%.c
	@echo [C obj]:   $^ -^> $@
	@$(ENSUREDIR)
	@$(CC) $(C_INCLUDES) $(C_DEFINES) $(C_FLAGS) -o $@ -c $^

# Build executable
$(BINDIR)/lepton.exe: $(OBJFILES) $(LIBFILES)
	@echo [link]:   $^ -^> $@
	@$(ENSUREDIR)
	$(CC) $(C_INCLUDES) $(C_DEFINES) $(C_FLAGS) $^ -o $@

.PHONY: clean
clean:
	@$(call RMDIR,$(BUILDDIR))
