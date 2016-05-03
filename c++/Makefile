# location of the Python header files

# location of the Boost Python include files and library
 
BOOST_INC = -I/usr/local/include
CC=g++
CCFLAGS=-Wall -std=c++11
LDFLAGS=-L/usr/local/lib -lpthread -lutil -lboost_system -lboost_iostreams -lboost_filesystem
SOURCES=$(wildcard *.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
TARGET=test

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CCFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp %.h
	$(CC) $(CCFLAGS) $(BOOST_INC) -fPIC -c $<

%.o: %.cpp
	$(CC) $(CCFLAGS) $(BOOST_INC) -fPIC -c $<

clean:
	rm -f *.o $(TARGET)
