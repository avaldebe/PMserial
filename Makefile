TEST = $(wildcard examples/debug/platformio.ini)
INI =  $(wildcard examples/*/platformio.ini)
.PHONY: all test $(INI)

test: $(TEST)
all:  $(INI)

$(INI):
	pio ci -l ./ -c $@ $(@D)