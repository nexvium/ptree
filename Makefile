# vim: ft=make ts=8 sts=8 sw=8 noet
#
CPPFLAGS :=
CXXFLAGS := -std=c++11 -pedantic-errors -Wall

ifeq ($(BUILD_OPT),YES)
  TYPE=_opt
  CXXFLAGS += -O2
else
  TYPE=_dbg
  CXXFLAGS += -ggdb3
  CPPFLAGS += -DDEBUG
endif

TARGET := ptree$(TYPE)
OBJDIR := objs$(TYPE)
DEPDIR := deps$(TYPE)

SOURCES := Procs.cpp ReadDir.cpp ptree.cpp
OBJECTS := $(patsubst %.cpp,%.o,$(SOURCES))

-include $(OBJS:.o=.d)

all: $(TARGET)

$(TARGET): $(OBJECTS:%=$(OBJDIR)/%)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(OBJECTS:%=$(OBJDIR)/%) -o $(TARGET)

$(OBJDIR) $(DEPDIR):
	mkdir -p $@

# build and object file *and* automatically create a dependency file
$(OBJDIR)/%.o: %.cpp | $(OBJDIR) $(DEPDIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $*.cpp -o $(OBJDIR)/$*.o
	@$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MM $*.cpp > $(DEPDIR)/$*.d
	@mv -f $(DEPDIR)/$*.d $(DEPDIR)/$*.d.tmp
	@sed -e 's|.*:|$(DEPDIR)/$*.o:|' < $(DEPDIR)/$*.d.tmp > $(DEPDIR)/$*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $(DEPDIR)/$*.d.tmp | fmt -1 | \
	  sed -e 's/^ *//' -e 's/$$/:/' >> $(DEPDIR)/$*.d
	@rm -f $(DEPDIR)/$*.d.tmp

.PHONY: clean
clean:
	rm -rf deps_* objs_* ptree_* *.o *.d

# eof
