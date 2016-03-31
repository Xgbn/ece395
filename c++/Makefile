# location of the Python header files

# location of the Boost Python include files and library
 
BOOST_INC = /usr/local/include
BOOST_LIB = /usr/local/lib
BOOST_LINK = -lboost_system -lboost_thread
CFLAGS = -Wall -std=c++11
INCLUDE = serial

# compile mesh classes
TARGET = test
 
$(TARGET): $(TARGET).o $(INCLUDE).o
	g++ $(CFLAGS) $(TARGET).o $(INCLUDE).o -L$(BOOST_LIB) $(BOOST_LINK) -o $(TARGET)
 
$(TARGET).o: $(TARGET).cpp
	g++ -I$(BOOST_INC) -fPIC -c $(TARGET).cpp

$(INCLUDE).o: $(INCLUDE).cpp
	g++ -I$(BOOST_INC) -fPIC -c $(INCLUDE).cpp

clean:
	rm -r *.o $(TARGET)