# makefile for mean_and_std_dev.cpp

CXX = g++
CXXFLAGS = -std=c++0x
CXXFLAGS += -Wall
CXXFLAGS += -ansi
CXXFLAGS += --pedantic-errors

OBJS = mean_and_std_dev.o

SRCS = mean_and_std_dev.cpp

mean_and_std_dev: ${OBJS} ${SRCS}
	${CXX} ${CXXFLAGS} ${SRCS} -o mean_and_std_dev

${OBJS}: ${SRCS}
	${CXX} ${CXXFLAGS} -c $(@:.o=.cpp)
