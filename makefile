#makefile
# example working make file for use of the class Student
# files: source files:  Main.cpp QuestionFactory.cpp AdditionFactory.cpp
# QuestionGenreSwitcher.cpp
#        header file:  QuestionFactory.h AdditionFactory.h
# QuestionGenreSwitcher.h
# executable file: Main
#

# first define target file : xxxxxxxx 
#              dependencies are the object files that build the program

Main: Main.o QuestionFactory.o AdditionFactory.o SubtractionFactory.o QuestionGenreSwitcher.o
	g++ -o Main Main.o QuestionFactory.o AdditionFactory.o SubtractionFactory.o QuestionGenreSwitcher.o

# now define how each object file is a target and list dependencies and how
#  to build that object file if any dependencies change

Main.o: Main.cpp 
	g++ -c Main.cpp

QuestionFactory.o: QuestionFactory.cpp QuestionFactory.h
	g++ -c QuestionFactory.cpp

AdditionFactory.o: AdditionFactory.cpp AdditionFactory.h
	g++ -c AdditionFactory.cpp

SubtractionFactory.o: SubtractionFactory.cpp SubtractionFactory.h
	g++ -c SubtractionFactory.cpp

QuestionGenreSwitcher.o: QuestionGenreSwitcher.cpp QuestionGenreSwitcher.h
	g++ -c QuestionGenreSwitcher.cpp

clean:
	rm -f Main Main.o QuestionFactory.o AdditionFactory.o SubtractionFactory.o QuestionGenreSwithcher.o


# that's all

