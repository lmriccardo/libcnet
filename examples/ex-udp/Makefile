CC=gcc -Wall -O3
INCLUDES=../../includes/
LIB=../../dist/static/
LIBS=-lpthread -lrt -lcnet
TARGET=udp

all: clean $(TARGET)

clean:
	rm -f $(TARGET)

$(TARGET) : $(TARGET).c
	sudo $(CC) -I$(INCLUDES) -g -o $(TARGET) $(TARGET).c -L$(LIB) $(LIBS)
	sudo chmod u+s $(TARGET)

memcheck: $(TARGET).c
	sudo $(CC) -I$(INCLUDES) -g -o $(TARGET) $(TARGET).c -L$(LIB) $(LIBS)

.PHONY: clean