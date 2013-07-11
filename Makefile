EXAMPLES=$(wildcard examples/*)
TESTS=$(wildcard tests/*)
MODE = release
PLATFORM = computer
PLUGINS = 
SCONS = `which scons`
SCONS += mode=$(MODE) platform=$(PLATFORM) PLUGINS=$(PLUGINS)

.PHONY: all clean install $(EXAMPLES)

default: all

all:
	@echo "== Building Qualia in mode $(MODE) =="
	$(SCONS)
	
clean:
	@echo "== Cleaning Qualia mode $(MODE) =="
	$(SCONS) --clean

install:
	@echo "== Installing Qualia in mode $(MODE) =="
	$(SCONS) install

examples: $(EXAMPLES)

$(EXAMPLES): all
	@echo "== Building example $@ in mode $(MODE) =="
	@cd $@ && $(SCONS)

tests: $(TESTS)

$(TESTS): all
	@echo "== Building test $@ in mode $(MODE) =="
	@cd $@ && $(SCONS)

