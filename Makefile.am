
AUTOMAKE_OPTIONS = gnu

EXTRA_DIST = doxyfile doc/images
SUBDIRS = zipios++


DOCTOOL = doxygen
DOCCONF = doxyfile

.PHONY : doc
doc :
	convert doc/images/zipios++.jpg doc/images/zipios++.eps
	$(DOCTOOL) $(DOCCONF)

.PHONY : devdoc
devdoc :
	$(DOCTOOL) $(DOCCONF)

# the first perl one-liner changes the robots meta tag so
# search engines will index the first page. The second provides
# a list of keywords that are relevant, so the search engine
# will list this page if someone types in these keywords.
.PHONY : webdoc
webdoc : doc
	$(MAKE) -C doc/latex pdf
	$(MAKE) -C doc/latex ps
	mv doc/latex/refman.pdf doc/html/
	mv doc/latex/refman.ps doc/html/
	cp doc/images/webgfx.gif doc/html/
	perl -p -i -e 's/content\=\"noindex\"/content\=\"index\,nofollow\"/gi' doc/html/index.html
	perl -p -i -e 's/\<meta name\=\"robots\"/\<meta name\=\"Keywords\" content\=\"Zip\, C++\, iostream\, library\, API\, decompression, VFS\, source\"\> \<meta name\=\"robots\"/gi' doc/html/index.html

# The URL http://sourceforge.net/project/?group_id=5418 is not parsed correctly
# by doxygen, that generates the link <a href="5418">, so this is just a
# dirty "fix"

# a quick and dirty target to make
# everything that goes on the web site.
# cp doc/images/webgfx.gif must be copied manually,
# as doxygen is too stupid to copy it from in from the
# images directory, because it doesn't use a doxygen tag
# to reference the image.
.PHONY : web
web : webdoc dist
