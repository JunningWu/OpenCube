DIRECTORIES = $(shell ls -F | grep "/")
#DIRECTORIES = $(shell ls -l | grep ^d | awk '{print $9}')
 
 
all:$(DIRECTORIES)
	$(foreach dir,$(DIRECTORIES),make -C $(dir);)
 
clean:
	$(foreach dir,$(DIRECTORIES),make -C $(dir) clean;)

distclean:
	$(foreach dir,$(DIRECTORIES),make -C $(dir) distclean;)
