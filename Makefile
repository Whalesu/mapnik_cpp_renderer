# inherit from env
CXX := $(CXX)
CC := $(CC)
CXXFLAGS := $(CXXFLAGS) -Wall -pedantic -g -v -lstdc++
CFLAGS := $(CFLAGS) -Wall -pedantic LDFLAGS := $(LDFLAGS)

# mapnik settings
MAPNIK_CXXFLAGS := $(shell mapnik-config --cflags)
MAPNIK_LDFLAGS := $(shell mapnik-config --libs)
MAPNIK_PLUGINDIR := $(shell mapnik-config --input-plugins)
LIBNAME := mapnik_renderer

all: $(LIBNAME)
$(LIBNAME): mapnik_renderer.cpp
	$(CXX) -o $(LIBNAME) $(SHARED_FLAG) mapnik_renderer.cpp src/image_provider.cpp src/image_provider_file.cpp src/image_provider_stream.cpp src/mapnik_c_api.cpp src/utils/util.cpp -I ./include $(LDFLAGS) $(CXXFLAGS) $(MAPNIK_CXXFLAGS) $(MAPNIK_LDFLAGS)
