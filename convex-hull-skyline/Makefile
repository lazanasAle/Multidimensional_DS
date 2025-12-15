PROGS = convex_hull kdbtree

all:
	for dir in $(PROGS); do \
		$(MAKE) -C $$dir all; \
	done

clean:
	for dir in $(PROGS); do \
		$(MAKE) -C $$dir clean; \
	done
