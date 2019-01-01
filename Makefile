# Makefile for MVEE
# Author: Chris Larson (2017)

# Local directories
ROOTDIR := $(shell pwd)
TESTDIR := $(ROOTDIR)/tests
BUILDDIR := $(ROOTDIR)/build
OBJDIR := $(BUILDDIR)/obj
LIBDIR := $(BUILDDIR)/lib
BINDIR := $(BUILDDIR)/bin

# Program name
SHARED_LIB := $(LIBDIR)/libmvee.so

# Compiler setup
CXX := gcc
OPTFLAGS := -o0
CXXFLAGS := --std=c++11 $(OPTFLAGS)
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

# Source file
SRC := \
$(ROOTDIR)/src/mvee.cpp

# Intermediate object filenames
OBJ := \
$(SRC:.c=$(BUILD_DIR)/.o)

# Resulting compilation rule
$(SHARED_LIB): $(OBJ)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) \
	-o $@ $^ $(LDFLAGS) $(LIBS)

# Cleanup
.PHONY: clean

clean:
	rm -rf $(BUILDDIR)/*
