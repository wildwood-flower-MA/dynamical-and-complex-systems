CXX = g++

CXXFLAGS = -std=c++20 -Wall -Wextra -g

LDFLAGS = -lgsl -lgslcblas -lm
TARGET = calculations
SRCS = lab8.cpp
DEPS = $(wildcard *.hpp)
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean