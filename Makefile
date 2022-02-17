CXX = g++
CXXFLAGS = -std=c++17 -Wall -MMD -g
EXEC = chess
SRC = .
SOURCES = $(wildcard $(SRC)/*.cc)
OBJECTS = ${SOURCES:.cc=.o}
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${DEPENDS} ${EXEC}