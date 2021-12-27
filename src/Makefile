BIN        := /usr/local/bin
IOCTL      := ioctl
DETACH     := detach

all: build

build: buildioctl builddetach

buildioctl:
	gcc -o $(IOCTL) $(IOCTL).c

builddetach:
	gcc -o $(DETACH) $(DETACH).c -lutil

clean:
	rm -f $(IOCTL) $(DETACH)

install: uninstall installioctl installdetach

installioctl:
	cp $(IOCTL) $(BIN)
	chmod a+x $(BIN)/$(IOCTL)

installdetach:
	cp $(DETACH) $(BIN)
	chmod a+x $(BIN)/$(DETACH)

uninstall:
	rm -f $(BIN)/$(IOCTL) $(BIN)/$(DETACH)
