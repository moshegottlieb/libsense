LDFLAGS+=-lsense
WAND_LDFLAGS=`pkg-config --libs MagickWand` 
CURL_LDFLAGS=`pkg-config --libs libcurl`
CFLAGS+=-g -O3 `pkg-config --cflags MagickWand` `pkg-config --cflags libcurl` 
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

TARGETS=$(BUILDDIR)/senseimage $(BUILDDIR)/sensepaint $(BUILDDIR)/sensetext $(BUILDDIR)/senserainbow $(BUILDDIR)/openweathermap

IMAGE_OBJ = $(BUILDDIR)/image.o

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

$(BUILDDIR)/senseimage: $(BUILDDIR)/senseimage.o $(IMAGE_OBJ)
	@echo [LINK] $@
	@$(CXX) $(CXXFLAGS) $(CFLAGS) $(BUILDDIR)/senseimage.o $(IMAGE_OBJ) $(LDFLAGS) $(WAND_LDFLAGS) -o $@

$(BUILDDIR)/senserainbow: $(BUILDDIR)/senserainbow.o
	@echo [LINK] $@
	@$(CXX) $(CXXFLAGS) $(CFLAGS) $(BUILDDIR)/senserainbow.o $(LDFLAGS) -o $@


$(BUILDDIR)/sensepaint: $(BUILDDIR)/sensepaint.o
	@echo [LINK] $@
	@$(CXX) $(CXXFLAGS) $(CFLAGS) $(BUILDDIR)/sensepaint.o $(LDFLAGS) $(WAND_LDFLAGS) -o $@

$(BUILDDIR)/sensetext: $(BUILDDIR)/sensetext.o
	@echo [LINK] $@
	@$(CC) $(CFLAGS) $(BUILDDIR)/sensetext.o $(LDFLAGS) -o $@

$(BUILDDIR)/openweathermap: $(BUILDDIR)/openweathermap.o $(IMAGE_OBJ)
	@echo [LINK] $@
	@$(CXX) $(CXXFLAGS) $(CFLAGS) $(BUILDDIR)/openweathermap.o $(IMAGE_OBJ) $(LDFLAGS) $(WAND_LDFLAGS) $(CURL_LDFLAGS) -o $@
