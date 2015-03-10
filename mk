#!/bin/sh
if test "$1" = "-d"
then
	# Rebuild documentation on the spot
	rm -rf ../BUILD/zipios/doc/zipios-doc-2.0*
	make -C ../BUILD/zipios/ zipios_Documentation
	make -C ../BUILD/zipios/ install
else
	make -C ../BUILD/zipios/
fi
