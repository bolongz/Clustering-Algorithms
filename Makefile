Make=make
subsystem:
	cd complete && $(MAKE)
	cd llord && $(MAKE)
	cd hamming && $(MAKE)
clean:
	cd complete && $(MAKE) clean
	cd llord && $(MAKE) clean
	cd hamming && $(MAKE) clean

