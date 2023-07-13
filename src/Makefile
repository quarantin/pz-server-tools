BIN    := /usr/local/bin
PZSTC  := pzstc
PZSTD  := pzstd
PZWRAP := pzwrap
DETACH := detach
CFLAGS := -Wall -Werror -Wextra -pedantic

all: build

build: buildpzst buildpzstd builddetach buildpzwrap

buildpzst:
	gcc $(CFLAGS) -o $(PZSTC) $(PZSTC).c

buildpzstd:
	gcc $(CFLAGS) -o $(PZSTD) $(PZSTD).c

builddetach:
	gcc $(CFLAGS) -DDETACH -o $(DETACH) $(DETACH).c -lutil

buildpzwrap:
	gcc $(CFLAGS) -DPZWRAP -o $(PZWRAP) $(DETACH).c -lutil

clean:
	rm -f $(PZSTC) $(PZSTD) $(DETACH)

install: uninstall clean build installpzst installpzstd installdetach installpzwrap

installpzst:
	cp $(PZSTC) $(BIN)
	chmod 755 $(BIN)/$(PZSTC)

installpzstd:
	cp $(PZSTD) $(BIN)
	chmod 755 $(BIN)/$(PZSTD)

installdetach:
	cp $(DETACH) $(BIN)
	chmod 755 $(BIN)/$(DETACH)

installpzwrap:
	rm -f $(BIN)/$(PZWRAP)
	cp $(PZWRAP) $(BIN)
	chmod 755 $(BIN)/$(PZWRAP)

uninstall:
	rm -f $(BIN)/$(PZSTC) $(BIN)/$(PZSTD) $(BIN)/$(DETACH) $(BIN)/$(PZWRAP)
