MAKEFILES = $(shell find . -maxdepth 2 -type f -name Makefile)
SUBDIRS   = $(filter-out ./,$(dir $(MAKEFILES)))

both_all:
	for dir in $(SUBDIRS); do \
		make -C $$dir all; \
	done

both_clean:
	for dir in $(SUBDIRS); do \
		make -C $$dir clean; \
	done

both_doc:
	for dir in $(SUBDIRS); do \
		make -C $$dir doc; \
	done

both_kill:
	for dir in $(SUBDIRS); do \
		make -C $$dir kill; \
	done
