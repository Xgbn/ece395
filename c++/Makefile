# location of the Python header files

 
# location of the Boost Python include files and library
 
BOOST_INC = /usr/local/include
BOOST_LIB = /usr/local/lib
 
# compile mesh classes
TARGET = test
 
$(TARGET): $(TARGET).o
	g++ -Wall $(TARGET).o -L/usr/local/lib -lserial -o $(TARGET)
 
$(TARGET).o: $(TARGET).cpp
	g++ -fPIC -c $(TARGET).cpp