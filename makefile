CC = g++

APPNAME	= sbflite

SRC 	:= sbflite.cpp Ethernet.cpp Inverter.cpp InfluxDB.cpp

CFLAGS 	:= -c -Wall -Wextra -Wconversion -O2 -Wno-unused-local-typedefs
LIBS   	:= pthread
LDFLAGS	:= -s

BINDIR 	:= bin/
OBJDIR	:= obj/
OBJECTS := $(SRC:%.cpp=$(OBJDIR)%.o)
TARGET	:= $(BINDIR)$(APPNAME)

$(OBJDIR)%.o: %.cpp
	$(CC) $^ $(CFLAGS) -o $@

$(TARGET): $(OBJECTS)
	$(CC) $^ $(LDFLAGS) -o $@ -Wl,-Bdynamic $(addprefix -l,$(LIBS))

.PHONY: clean
clean:
	rm $(OBJDIR)*

# $(output) $(source)
#	$(CC) -o $(output) $(CFLAGS) $(addprefix -l,$(LIBS)) $(source)

