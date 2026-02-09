# Compiler
CXX = g++

# Compiler Flags
# -c : just do preprocess; compile and assemble into object files
# -g : adds debugging information to the executable file
# -Wall : turns on most, but not  all, compiler warnings
CXXFLAGS = -c -g -Wall

# Program Exe File Name
PROGRAM_NAME = Testing.out

# Library File Location
# Enter the file path to the libraries needed to run the code here:
Lib_Location = ../Libraries/FormattingC++/CEWBFormatting.cpp

# File List
# CPP_FILES = CSideInteract.cpp
CPP_FILES = LibraryTest.cpp 

build:
	$(CXX) $(CXXFLAGS) $(CPP_FILES) $(Lib_Location)
	$(CXX) -o $(PROGRAM_NAME) *.o

run:
	./$(PROGRAM_NAME)

clean:
	rm -f $(PROGRAM_NAME) *.o