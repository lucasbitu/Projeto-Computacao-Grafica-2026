CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic -I include
LDLIBS := -lGL -lGLU -lglut

TARGET := simulador

SRCS := \
	src/main.cpp \
	src/texturas_util.cpp \
	src/camera.cpp \
	src/moveis.cpp \
	src/cenario.cpp \
	src/render/primitives.cpp \
	src/scene/house.cpp \
	src/scene/internal_walls.cpp \
	src/scene/windows.cpp \
	src/scene/doors.cpp \
	src/furniture/dining.cpp \
	src/furniture/living_room.cpp \
	src/furniture/kitchen.cpp \
	src/furniture/bathroom.cpp \
	src/furniture/bedroom.cpp

OBJS := $(SRCS:.cpp=.o)

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDLIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) $(OBJS)
