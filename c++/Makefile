# location of the Python header files

# location of the Boost Python include files and library
 
BOOST_INC = /usr/local/include
BOOST_LIB = /usr/local/lib
 
# compile mesh classes
TARGET = test
 
$(TARGET): $(TARGET).o
	g++ -Wall $(TARGET).o -L$(BOOST_LIB) -lboost_system -lboost_thread -o $(TARGET)
 
$(TARGET).o: $(TARGET).cpp
	g++ -I$(PYTHON_INCLUDE) -I$(BOOST_INC) -fPIC -c $(TARGET).cpp

clean:
	rm -r *.o $(TARGET)