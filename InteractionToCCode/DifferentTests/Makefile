# Compiler
CXX = g++

# Compiler Flags
# -c : just do preprocess; compile and assemble into object files
# -g : adds debugging information to the executable file
# -Wall : turns on most, but not  all, compiler warnings
CXXFLAGS = -c -g -Wall

# Program Exe File Name
PROGRAM_NAME = SendDataToTwoFiles.out

# File List
CPP_FILES = SendDataToTwoFiles.cpp

build:
	$(CXX) $(CXXFLAGS) $(CPP_FILES)
	$(CXX) -o $(PROGRAM_NAME) *.o

run:
	./$(PROGRAM_NAME)

clean:
	rm -f $(PROGRAM_NAME) *.o