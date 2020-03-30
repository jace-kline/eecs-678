quash: main.o parse.o types.o environment.o filepath.o user-interact.o
	g++ -std=c++11 -g -Wall user-interact.o filepath.o environment.o parse.o types.o main.o -o quash

main.o: main.cpp parse.h user-interact.h environment.h
	g++ -std=c++11 -g -Wall -c main.cpp

parse.o: parse.h parse.cpp types.h
	g++ -std=c++11 -g -Wall -c parse.cpp

types.o: types.h types.cpp
	g++ -std=c++11 -g -Wall -c types.cpp

environment.o: environment.h environment.cpp filepath.h
	g++ -std=c++11 -g -Wall -c environment.cpp

filepath.o: filepath.h filepath.cpp
	g++ -std=c++11 -g -Wall -c filepath.cpp

user-interact.o: user-interact.h user-interact.cpp
	g++ -std=c++11 -g -Wall -c user-interact.cpp

clean:
	rm prog *.o