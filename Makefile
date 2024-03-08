CXX := mpic++
CXXFLAGS := -g -std=c++11

SOURCES := cGame.cpp cBoardState.cpp chessMPI.cpp cHash.cpp
OBJECTS := $(SOURCES:.cpp=.o)

TARGET := chessMPI

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

pvp:
	mpirun -x TERM -hostfile hostfile $(TARGET)

pva:
	mpirun -x TERM -hostfile hostfile $(TARGET) pva

ava:
	mpirun -x TERM -hostfile hostfile $(TARGET) ava

ava2:
	mpirun -np 2 -x TERM -hostfile hostfile $(TARGET) ava

valgrind:
	mpirun -x TERM -hostfile hostfile valgrind ./$(TARGET) ava

valgrind2:
	mpirun -np 2 -x TERM -hostfile hostfile valgrind ./$(TARGET) ava

clean:
	rm -f $(OBJECTS) $(TARGET)
