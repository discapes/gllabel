CXX = c++
LDLIBS = -lfreetype -lGLEW -lglfw -lGL
INCLUDES = -Iinclude -I/usr/include/freetype2
WARNINGS = -Wall -Wextra
O = -Ofast
CXXFLAGS = -std=gnu++20
override CXXFLAGS += $(O) $(WARNINGS) $(INCLUDES)

rundemo: demo
	./demo
demo: demo.o libgllabel.a
	$(CXX) demo.o $(LDFLAGS) -L. -lgllabel $(LDLIBS) -o demo


objects = lib/gllabel.o

libgllabel.so: override LDFLAGS += -shared
libgllabel.so: override CXXFLAGS += -fPIC
libgllabel.so: $(objects)
	$(CXX) $(objects) $(LDFLAGS) $(LDLIBS) -o libgllabel.so

libgllabel.a: $(objects)
	$(AR) rcs libgllabel.a $(objects)

lib/gllabel.o: include/gllabel/gllabel.hpp
include/gllabel/gllabel.hpp : include/gllabel/glwrappers.hpp

.PHONY: clean
clean:
	rm $(objects) libgllabel.so libgllabel.a demo.o demo
