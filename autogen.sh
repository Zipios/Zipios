#!/bin/sh

aclocal
autoconf
#touch NEWS README AUTHORS ChangeLog
autoheader
automake --add-missing
