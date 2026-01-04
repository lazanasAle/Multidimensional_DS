PROGS = convex-hull-skyline interval-segment-tree Sweep_line

all:
	for dir in $(PROGS); do \
		$(MAKE) -C $$dir all; \
	done

clean:
	for dir in $(PROGS); do \
		$(MAKE) -C $$dir clean; \
	done
