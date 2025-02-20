.PHONY: download download-all build build-all install install-all install-clear

download:
ifndef TOKEN
	go run ./scripts/download
else
	go run ./scripts/download --token=$(TOKEN)
endif

download-all:
ifndef TOKEN
	go run ./scripts/download --all
else
	go run ./scripts/download --token=$(TOKEN) --all 
endif

build:
	go run ./scripts/build

build-all:
	go run ./scripts/build --all

install:
	go run ./scripts/install

install-all:
	go run ./scripts/install --all

install-clear:
	go run ./scripts/install --clear
