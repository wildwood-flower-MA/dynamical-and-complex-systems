CXX = g++
CXXFLAGS = -Wall -O2
LIBS = -lgsl -lgslcblas -lm
TARGET = lab2

all: $(TARGET)

$(TARGET): lab2.cpp
	$(CXX) $(CXXFLAGS) lab2.cpp -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET)