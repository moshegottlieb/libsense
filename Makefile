
SOURCES = $(wildcard *.c)
OBJ = $(patsubst %.c,$(BUILDDIR)/%.o,$(SOURCES))
PREFIX?=/usr
CFLAGS+=-g -fPIC -O3
VERSION_MAJ=1
VERSION_MIN=0.1
TARGET_NAME=libsense
BUILDDIR=build
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.d
MAKE_PID := $(shell echo $$PPID)
JOB_FLAG := $(filter -j%, $(subst -j ,-j,$(shell ps T | grep "^\s*$(MAKE_PID).*$(MAKE)")))
JOBS     := $(subst -j,,$(JOB_FLAG))
ifeq ($(OS),Darwin)
	CPU_COUNT:=$(shell sysctl -n hw.logicalcpu_max)
	SO_SUFFIX=dylib
else
	CPU_COUNT:=$(shell grep -c ^processor /proc/cpuinfo)
	SO_SUFFIX=so
endif

TARGET_SO=$(TARGET_NAME).$(SO_SUFFIX)
TARGET_SO_NAME=$(TARGET_SO).$(VERSION_MAJ)
TARGET_FULL_NAME=$(TARGET_SO_NAME).$(VERSION_MIN)
TARGET=$(BUILDDIR)/$(TARGET_FULL_NAME)

MAKE:=$(MAKE) --no-print-directory $(JOB_FLAGS)

all: lib

DEPDIR := .d
$(shell mkdir -p $(DEPDIR) >/dev/null)
$(shell mkdir -p $(BUILDDIR) >/dev/null)

$(OBJ): $(BUILDDIR)/%.o : %.c $(DEPDIR)/%.d	
	@echo [C] $(notdir $<)
	@$(COMPILE.c) $(OUTPUT_OPTION) $(DEPFLAGS) $< 
	$(POSTCOMPILE)

$(DEPDIR)/%.d: ;

.PRECIOUS: $(DEPDIR)/%.d

.PHONY: clean test

-include $(patsubst %,$(DEPDIR)/%.d,$(basename $(SOURCES)))
	
$(TARGET): $(OBJ)
	@echo [LINK] $(TARGET_NAME)
	@$(CC) $(LDFLAGS) $(CFLAGS) -shared -Wl,-soname,$(TARGET_SO_NAME) -o $(TARGET) $(OBJ)
	@chmod -x $(TARGET)
	
lib: $(TARGET)

test: $(TARGET)
	@$(MAKE) -C test/

doc:
	@doxygen doxygen.cfg

install: $(TARGET) real_uninstall
	@echo [INSTALL] $(TARGET_NAME)
	@mkdir -p "$(PREFIX)/include/sense"
	@cp $(TARGET) "$(PREFIX)/lib/"
	@rm -f "$(PREFIX)/lib/$(TARGET_SO_NAME)"
	@rm -f "$(PREFIX)/lib/$(TARGET_SO_NAME).*"
	@rm -f "$(PREFIX)/lib/$(TARGET_SO)"
	@ln -s "$(PREFIX)/lib/$(TARGET_FULL_NAME)" "$(PREFIX)/lib/$(TARGET_SO_NAME)"
	@ln -s "$(PREFIX)/lib/$(TARGET_FULL_NAME)" "$(PREFIX)/lib/$(TARGET_SO)"
	@cp *.h *.hpp "$(PREFIX)/include/sense"

uninstall: uninstall_banner real_uninstall

uninstall_banner:
	@echo [UNINSTALL] $(TARGET_NAME)

real_uninstall:
	@rm -rf "$(PREFIX)/include/sense" "$(PREFIX)/lib/$(TARGET_FULL_NAME)" "$(PREFIX)/lib/$(TARGET_SO_NAME)" "$(PREFIX)/lib/$(TARGET_SO)"

clean:
	@rm -rf build/* .d/* docs/*
