# Makefile --- 
#
# Copyright (C)  RTI im. Mintca
# Copyright (C)  Egorov N.V.
#
# Author: Egorov N.V. <egorov@rti-mints.ru>
# $Id: Makefile,v 971be88023c0  2012/10/30 13:16:22  egorov@rti-mints.ru $
#

# Данная программа разработана в ОАО "РТИ имени Минтца".
#


SELF_DIR=$(shell pwd | xargs basename)
ifndef BIN_DIR
BIN_DIR=$(SELF_DIR)
endif
INCLUDEDIR =
BIN_PREFIX = ../$(BIN_DIR)
SOURCES = $(wildcard *.c)
OBJECTS = $(patsubst %.c,%.o,$(SOURCES))
DEPENDENCES = $(patsubst %.c,%.dep,$(SOURCES))
CFLAGS  += -Wall -pipe -O2 $(INCLUDEDIR) -DNDEBUG
TARGET= $(BIN_PREFIX)/_test_COM
DAEMON_TARGET=$(BIN_PREFIX)/_test_COM_daemon



all: $(TARGET) $(DAEMON_TARGET)
	-cp _test_COM_daemon $(DAEMON_TARGET)
	chmod +x $(DAEMON_TARGET)

-include $(DEPENDENCES)

.PHONY: compile
compile: $(OBJECTS)

install:
	mkdir -p $(DESTDIR)/bin
	install -m 7555 $(TARGET) $(DESTDIR)/bin
	install -m 7555 $(DAEMON_TARGET) $(DESTDIR)/bin

$(TARGET): $(OBJECTS) 
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

$(OBJECTS): Makefile 


%.dep: %.c
	$(CC) $(CFLAGS) -MM -o $@ $^
.PHONY:echo clean dependences distclean run
echo:
	@echo CC = $(CC)
	@echo INCLUDEDIR = $(INCLUDEDIR)
	@echo SOURCES = $(SOURCES)
	@echo OBJECTS = $(OBJECTS)
	@echo DEPENDENCES = $(DEPENDENCES)
	@echo CFLAGS = $(CFLAGS)

clean:
	rm -f $(OBJECTS)

distclean: clean
	rm -f $(DEPENDENCES)
	rm -f $(TARGET) 
	chmod -x $(DAEMON_TARGET)
ifneq ($(BIN_DIR),$(SELF_DIR))
	rm -f $(DAEMON_TARGET)
endif
	rm -f *~
	rm -f \#*

run: $(TARGET)
	./$(TARGET) $(PARAMETRS)

$(BIN_PREFIX)/%: %
	cp $< $@
	chmod +x $@


# end
