#!/bin/sh
#
# You can consider this file as being in the public domain.
# Sample script used to build zipios after you ran cmake

PROCESSORS=`nproc`

# Determine the location of the BUILD folder
if test -d ../../../BUILD/contrib/zipios
then
	BUILD=../../../BUILD/contrib/zipios
else
	BUILD=../BUILD/zipios
fi

case $1 in
"-l")
	make -C ${BUILD} 2>&1 | less -SR
	;;

"-d")
	rm -rf ${BUILD}/doc/zipios-doc-*.gz
	make -C ${BUILD} zipios_Documentation
	make -C ${BUILD} install
	;;

"-i")
	make -j${PROCESSORS} -C ${BUILD} install
	;;

"-t")
	(
		if make -j${PROCESSORS} -C ${BUILD}
		then
			shift
			${BUILD}/tests/zipios_tests $*
		fi
	) 2>&1 | less -SR
	;;

"")
	make -j${PROCESSORS} -C ${BUILD}
	;;

*)
	echo "error: unknown command line option \"$1\""
	;;

esac
