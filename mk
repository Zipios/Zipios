#!/bin/sh
#
# You can consider this file as being in the public domain.
# Sample script used to build zipios after you ran cmake

PROCESSORS=`nproc`
TYPE=Debug
SOURCE=`pwd`
PROJECT=`basename ${SOURCE}`

# Determine the location of the BUILD folder
if test -d ../../BUILD/${TYPE}/contrib/${PROJECT}
then
	BUILD=../../BUILD/${TYPE}/contrib/${PROJECT}
else
	BUILD=../BUILD/${PROJECT}
fi

PROJECT_TMPDIR="${BUILD}/tmp"

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
			${BUILD}/tests/unittest \
				--tmp-dir "${PROJECT_TMPDIR}" \
				--source-dir "${SOURCE}" \
				--progress \
					$*
		fi
	) 2>&1 | less -SR
	;;

"-v")
	VERBOSE=1 make -j${PROCESSORS} -C ${BUILD}
	;;

"")
	make -j${PROCESSORS} -C ${BUILD}
	;;

*)
	echo "error: unknown command line option \"$1\""
	;;

esac
