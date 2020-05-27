# Config Begin
CXX = g++
CXXFLAGS = -std=gnu++11 -Wall -Wextra
SOURCES = imgsys_util.cpp imgsys_cameras.cpp
SOURCES_GSTREAMER = imgsys.cpp
OUTPUT = img_sys
OBJECT_DIR = obj
# Config End

OBJECTS = $(SOURCES:%.cpp=$(OBJECT_DIR)/%.o)
OBJECTS_GSTREAMER = $(SOURCES_GSTREAMER:%.cpp=$(OBJECT_DIR)/gst_%.o)
GSTREAMER_CFLAGS = `pkg-config gstreamer-1.0 --cflags`
GSTREAMER_LIBS = `pkg-config gstreamer-1.0 --libs`

# Targets
all: makeDirs $(OBJECTS) $(OBJECTS_GSTREAMER) $(OUTPUT)

makeDirs:
	mkdir -p $(OBJECT_DIR)

$(OBJECT_DIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJECT_DIR)/gst_%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(GSTREAMER_CFLAGS) -c -o $@ $< $(GSTREAMER_LIBS)

$(OUTPUT):
	$(CXX) $(CXXFLAGS) $(GSTREAMER_CFLAGS) -o $@ $(OBJECT_DIR)/*.o $(GSTREAMER_LIBS)

# Phony targets
.PHONY: clean wipeSD mac
clean:
	rm -rf $(OBJECT_DIR)
	rm -f $(OUTPUT)

wipeSD:
	rm -f /media/nvidia/sd/*

# The version of g++ that ships with MacOS is actually just clang (llvm). I installed g++9
# to stay consistent with the Jetson so when compiling from my mac I can just say "make mac"
mac: CXX = g++-9
mac: all