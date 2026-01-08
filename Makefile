PROGS = convex-hull-skyline interval-segment-tree Sweep_line 3d-rtree\(trajectories\)

all:
	for dir in $(PROGS); do \
		$(MAKE) -C $$dir all; \
	done

clean:
	for dir in $(PROGS); do \
		$(MAKE) -C $$dir clean; \
	done
