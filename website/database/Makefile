prefix := /usr/local
CCFLAGS=-Ofast -l sqlite3

# define all programs
PROGRAMS = main
SOURCES = ${PROGRAMS:=.cpp}


all: ${PROGRAMS}

${PROGRAMS}: ${SOURCES}
	g++ -o $@ -Wall -I../ $@.cpp ${CCFLAGS}

clean:
	rm -rf $(PROGRAMS)