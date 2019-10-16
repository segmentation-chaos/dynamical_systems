INCLUDE_DIR=./include
CXXFLAGS=-Wall -I$(INCLUDE_DIR) #-Werror -O3
VPATH=./src

TARGETS=dyn_sys

DEPENDENCIES=main.cpp maps.cpp features.cpp

build: $(TARGETS)

$(TARGETS): $(DEPENDENCIES)
	$(CXX) $(CXXFLAGS) -o dyn_sys $^

.PHONY: clean
clean:
	-rm -f $(TARGETS)
