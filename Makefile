all: build

build:
	make -C ucode
	mv ucode/ucode tests/ucode