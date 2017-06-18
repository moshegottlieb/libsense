LDFLAGS+=-lsense `pkg-config --libs MagickWand`
CFLAGS+=-g `pkg-config --cflags MagickWand`
CXXFLAGS+=-std=c++11
BUILDDIR=build
SOURCESCXX = $(wildcard *.cpp)
SOURCESC = $(wildcard *.c)
OBJC = $(patsubst %.c,$(BUILDDIR)/%.o,$(SOURCESC))
OBJCXX = $(patsubst %.cpp,$(BUILDDIR)/%.o,$(SOURCESCXX)) 

DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.d
DEPDIR := .d
$(shell mkdir -p $(DEPDIR) >/dev/null)
$(shell mkdir -p $(BUILDDIR) >/dev/null)

TARGETS=senseimage sensepaint sensetext

all: $(TARGETS)

clean:
	@rm -f $(BUILDDIR)/* $(TARGETS)

$(OBJCXX): $(BUILDDIR)/%.o : %.cpp $(DEPDIR)/%.d
	@echo [C++] $(notdir $<)
	@$(COMPILE.cpp) $(CXXFLAGS) $(CFLAGS) $(OUTPUT_OPTION) $(DEPFLAGS) $< 
	$(POSTCOMPILE)

$(OBJC): $(BUILDDIR)/%.o : %.c $(DEPDIR)/%.d
	@echo [C] $(notdir $<)
	@$(COMPILE.cpp) $(CFLAGS) $(OUTPUT_OPTION) $(DEPFLAGS) $< 
	$(POSTCOMPILE)


$(DEPDIR)/%.d: ;

.PRECIOUS: $(DEPDIR)/%.d

-include $(patsubst %,$(DEPDIR)/%.d,$(basename $(SOURCESC)))

.PHONY: clean

senseimage: $(OBJC) $(OBJCXX)
	@echo [LINK] $@
	@$(CXX) $(CXXFLAGS) $(CFLAGS) $(BUILDDIR)/senseimage.o $(LDFLAGS) -o $@

sensepaint: $(OBJC) $(OBJCXX)
	@echo [LINK] $@
	@$(CXX) $(CXXFLAGS) $(CFLAGS) $(BUILDDIR)/sensepaint.o $(LDFLAGS) -o $@

sensetext: $(OBJC) $(OBJCXX)
	@echo [LINK] $@
	@$(CC) $(CFLAGS) $(BUILDDIR)/sensetext.o $(LDFLAGS) -o $@
