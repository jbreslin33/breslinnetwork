#makefile
# networking for a game
# files: source files:  Main.cpp GameServer.cpp ListenServer.cpp
#
#        header file:  GameServer.h ListenServer.h
#
# executable file: Main
#

# first define target file : xxxxxxxx
#              dependencies are the object files that build the program

Main: Main.o GameServer.o ListenServer.o
	g++ -o Main Main.o GameServer.o ListenServer.o

# now define how each object file is a target and list dependencies and how
#  to build that object file if any dependencies change

Main.o: Main.cpp
	g++ -c Main.cpp

GameServer.o: GameServer.cpp ListenServer.h
	g++ -c GameServer.cpp

ListenServer.o: ListenServer.cpp GameServer.h
	g++ -c ListenServer.cpp


clean:
	rm -f Main Main.o GameServer.o ListenServer.o


# that's all

