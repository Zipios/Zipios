#!/bin/sh

libtoolize --copy --force
aclocal
autoconf
#touch NEWS README AUTHORS ChangeLog
autoheader
automake --add-missing
