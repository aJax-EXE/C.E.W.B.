# compiler name
CXX = g++

# C++ compiler flags
# -c : just do preprocess; compile and assemble into object files
# -g : adds debugging information to the executable file
# -Wall : turns on most, but not  all, compiler warnings
# -std=c++17 : uses c++17 standard
# put all the required flags separated by space
CXXFLAGS = -c -g -Wall -std=c++17

# the build target executable file; .exe is not required
PROGRAM = LibraryTest.out

# Library File Location
# Enter the file path to the libraries needed to run the code here:
# Needs to be the .cpp file that ends up here
Lib_Location = CEWBFormatting.cpp

# list all .cpp files separated by space
CPPFILES = LibraryTest.cpp 

# make all rule/default rule that'll be run when we run "make"
# first compile each file to object file
# then make a single binary program
all:
	$(CXX) $(CXXFLAGS) $(CPPFILES) $(Lib_Location)
	$(CXX) -o $(PROGRAM) *.o

# make clean rule
# delete object and binary files
clean:
	rm -f $(PROGRAM) *.o

# FIXME
# Add a rule to run the program
run:
	./$(PROGRAM)

test:
	./$(PROGRAM) test