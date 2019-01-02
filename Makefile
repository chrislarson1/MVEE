# Makefile for MVEE
# Author: Chris Larson (2017)

# DIRECTORIES
ROOTDIR := $(shell pwd)
BUILDDIR := $(ROOTDIR)/build
OBJDIR := $(BUILDDIR)/obj
LIBDIR := $(BUILDDIR)/lib
BINDIR := $(BUILDDIR)/bin
PREFIX := /usr/local

# COMPILER FLAGS
CXX := gcc
CXXFLAGS := -fPIC --std=c++11 -o0 -g
LDFLAGS := \
-L/usr/lib \
-L/usr/local/lib \
-L/usr/lib/x86_64-linux-gnu
INCLUDES := \
-I$(ROOTDIR)/include \
-I/usr/include \
-I/usr/local/include \
-I/usr/include/eigen3 \
-I/usr/include/boost
LIBS := \
-lstdc++ \
-lboost_system \
-lboost_chrono \
-lboost_thread \
-lboost_filesystem \
-lm \
-lpthread

# SHARED LIB
SHARED_LIB := $(LIBDIR)/libmvee.so

# SOURCES
SRC := $(ROOTDIR)/src/mvee.cpp

# OBJECTS
OBJ := $(SRC:.c=$(OBJDIR)/.o)

# COMPILATION RULE
$(SHARED_LIB): $(OBJ)
	@mkdir -p $(dir $@)
	$(CXX) -shared -fPIC $(CXXFLAGS) $(INCLUDES) \
	-o $@ $^ $(LDFLAGS) $(LIBS)

# CLEAN
.PHONY: clean
clean:
	rm -rf $(BUILDDIR)/*

# INSTALL
.PHONY: install
install: $(SHARED_LIB)
	cp $(SHARED_LIB) $(DESTDIR)$(PREFIX)/lib/libmvee.so
	chmod 644 $(DESTDIR)$(PREFIX)/lib/libmvee.so
	mkdir -p $(DESTDIR)$(PREFIX)/include/mvee
	cp include/mvee.hpp $(DESTDIR)$(PREFIX)/include/mvee/mvee.hpp
	cp include/utils.hpp $(DESTDIR)$(PREFIX)/include/mvee/utils.hpp

# UNINSTALL
.PHONY: uninstall
uninstall:
	rm -f $(DESTDIR)$(PREFIX)/lib/libmvee.so
	rm -r -f $(DESTDIR)$(PREFIX)/include/mvee