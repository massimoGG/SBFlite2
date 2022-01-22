CC = g++

source = sbflite.c Ethernet.cpp Inverter.cpp Prometheus.cpp
output = sbflite
LIBS   = bluetooth
CFLAGS = -c -Wall -Wextra -Wconversion -O2 -Wno-unused-local-typedefs

sbflite : $(output) $(source) 
	$(CC) -o $(output) $(CFLAGS) $(addprefix -l,$(LIBS)) $(source)

.PHONY: clean
clean :
	rm $(output)
