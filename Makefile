CFLAGS=-g
LFLAGS=-Iheaders/
all: cpu_utilization

BUILD=./build-obj
SRCDIR=./src
HEADERSDIR=./headers

OBJ=$(BUILD)/main.o $(BUILD)/cpu_utilization.o
SRC=$(SRCDIR)/main.c $(SRCDIR)/cpu_utilization.c
HEADERS=$(HEADERSDIR)/cpu_utilization.h

cpu_utilization: $(OBJ)
	gcc $(CFLAGS) $^ -o cpu_utilization

$(BUILD)/cpu_utilization.o: $(SRCDIR)/cpu_utilization.c $(HEADERS)
	gcc -o $@ -c $< $(CFLAGS) $(LFLAGS)
$(BUILD)/main.o: $(SRCDIR)/main.c $(HEADERS)
	gcc -o $@ -c $< $(CFLAGS) $(LFLAGS)

clean:
	rm build-obj/*.o ./cpu_utilization

exe: cpu_utilization
	./cpu_utilization
