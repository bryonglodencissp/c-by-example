HEADER=test.h
HEADER_PUBLIC=$(HEADER).pub
HEADER_PRIVATE=$(HEADER).priv

all: $(HEADER_PRIVATE) $(HEADER_PUBLIC)

$(HEADER_PUBLIC): $(HEADER)
	gcc -C -dD -E $< -o $@
	sed -i '/^#.*/d' $@     # Remove gcc preprocessor comments
	sed -i '/^$$/d' $@      # Remove empty lines
	sed -i 's/^ \+/\t/g' $@ # Remove leading spaces by one tab

$(HEADER_PRIVATE): $(HEADER)
	gcc -DINTERNAL -C -dD -E $< -o $@
	sed -i '/^#.*/d' $@     # Remove gcc preprocessor comments
	sed -i '/^$$/d' $@      # Remove empty lines
	sed -i 's/^ \+/\t/g' $@ # Remove leading spaces by one tab

clean:
	rm -f $(HEADER_PUBLIC)
	rm -f $(HEADER_PRIVATE)
