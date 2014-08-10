GAME_CPP := $(wildcard game/*.cpp)
GAME_HPP := $(wildcard game/*.Hpp)
GAME_OBJ := $(GAME_CPP:game/%.cpp=obj/%.obj)
EXE_FILE := app

CXXFLAGS := -Wall -std=c++1y -I. -Ilibs/glfw/include
CXX := clang++
LDFLAGS := -Llibs -lglfw3 -lGLEW -lpthread -ljpeg
OS := $(shell uname -s)

ifeq ($(OS), Linux)
	LDFLAGS += -lGL -lGLU -lX11 -lXxf86vm -lXi -lXrandr
endif
ifeq ($(OS), Darwin)
	LDFLAGS += -framework Cocoa -framework OpenGL -framework CoreVideo -framework IOKit
endif

ifeq ($(RELEASE),1)
	CXXFLAGS += -O3
else
	CXXFLAGS += -g
endif

ifeq ($(ASAN), 1)
	CXXFLAGS += -fsanitize=address
	LDFLAGS += -fsanitize=address
endif


.PHONY: clean

$(EXE_FILE): $(GAME_OBJ)
	$(CXX) -o $@ $(GAME_OBJ) lumina/lumina.a $(LDFLAGS)

obj/%.obj: game/%.cpp $(GAME_HPP) | obj
	$(CXX) $(CXXFLAGS) -o $@ -c $< 


obj:
	mkdir obj

clean:
	rm -rf obj/ 
	rm -f $(EXE_FILE)