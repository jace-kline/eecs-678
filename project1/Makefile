quash: main.o execute.o parse.o types.o environment.o job-handler.o filepath.o user-interact.o
	g++ -std=c++11 -g -Wall user-interact.o filepath.o job-handler.o environment.o types.o parse.o execute.o main.o -o quash

main.o: main.cpp parse.h user-interact.h execute.h
	g++ -std=c++11 -g -Wall -c main.cpp

execute.o: execute.h execute.cpp parse.h job-handler.h
	g++ -std=c++11 -g -Wall -c execute.cpp

parse.o: parse.h parse.cpp types.h
	g++ -std=c++11 -g -Wall -c parse.cpp

types.o: types.h types.cpp
	g++ -std=c++11 -g -Wall -c types.cpp

environment.o: environment.h environment.cpp filepath.h
	g++ -std=c++11 -g -Wall -c environment.cpp

job-handler.o: job-handler.h job-handler.cpp
	g++ -std=c++11 -g -Wall -c job-handler.cpp

filepath.o: filepath.h filepath.cpp
	g++ -std=c++11 -g -Wall -c filepath.cpp

user-interact.o: user-interact.h user-interact.cpp
	g++ -std=c++11 -g -Wall -c user-interact.cpp

clean:
	rm prog *.o quash