
AUTOMAKE_OPTIONS = gnu

EXTRA_DIST = doxyfile doc/images
SUBDIRS = src


DOCTOOL = doxygen
DOCCONF = doxyfile

.PHONY : doc
doc :
	cd doc/images
	convert zipios++.png zipios++.eps
	cd -
	$(DOCTOOL) $(DOCCONF)

.PHONY : devdoc
devdoc :
	$(DOCTOOL) $(DOCCONF)

# the 'sed' part changes the robots meta tag so
# search engines will index the first page
.PHONY : webdoc
webdoc : doc
	$(MAKE) -C doc/latex pdf
	$(MAKE) -C doc/latex ps
	mv doc/latex/refman.pdf doc/html/
	mv doc/latex/refman.ps doc/html/
	cp doc/images/webgfx.gif doc/html/
	cp ChangeLog doc/html/
	cat doc/html/index.html | sed "s/content=\"noindex\"/content=\"index,nofollow\"/g" > doc/html/idx.html
	mv doc/html/idx.html doc/html/index.html

# a quick and dirty target to make
# everything that goes on the web site.
# cp doc/images/webgfx.gif must be copied manually,
# as doxygen is too stupid to copy it from in from the
# images directory, because it doesn't use a doxygen tag
# to reference the image.
.PHONY : web
web : webdoc dist
	-rm -rf $(PACKAGE)
	mv doc/html $(PACKAGE)
	mv $(PACKAGE)-$(VERSION).tar.gz $(PACKAGE)/

#	gtar cvfz $(PACKAGE)web.tar.gz $(PACKAGE)
#	-rm -rf $(PACKAGE) 


