include Makefile.inc

SAMPLE_DATA=0001-shellDataModule.bin

all: shellCodeModule shellDataModule

shellCodeModule:
	cd ShellCodeModule; make

shellDataModule:
	printf "This is shell data." >> $(SAMPLE_DATA) && dd if=/dev/zero bs=1 count=1 >> $(SAMPLE_DATA)

clean:
	cd ShellCodeModule; make clean
	rm -rf *.bin


.PHONY: shellCodeModule all clean
