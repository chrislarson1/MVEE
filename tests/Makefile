# Makefile for MVEE
# Author: Chris Larson (2017)

# DIRECTORIES
ROOTDIR := $(shell pwd)
BUILDDIR := $(ROOTDIR)/build

# EXECUTABLE
EXECUTABLE := $(BUILDDIR)/testmvee

# COMPILER FLAGS
CXX := gcc
CXXFLAGS := --std=c++11 -o0
LDFLAGS := \
-L/usr/lib \
-L/usr/local/lib \
-L/usr/lib/x86_64-linux-gnu
INCLUDES := \
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
-lpthread \
-lmvee

# SOURCES
SRC := \
$(ROOTDIR)/test.cpp

# OBJECTS
OBJ := \
$(SRC:.c=$(BUILD_DIR)/.o)

# COMPILATION RULE
$(EXECUTABLE): $(OBJ)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) \
	-o $@ $^ $(LDFLAGS) $(LIBS)

# CLEAN
.PHONY: clean
clean:
	rm -rf $(BUILDDIR)/*