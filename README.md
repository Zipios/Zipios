# Introduction

Zipios++ is a small C++ library for reading and writing zip files. The structure and public interface are based (somewhat loosely) on the java.util.zip package. The streams created to access the individual entries in a zip file are based on the standard iostream library.

Zipios++ also provides a way for an application to support files from multiple sources (e.g. from zip files or from ordinary directories) transparently.

The source code is released under the GNU Lesser General Public License (LGPL).


# Dependencies

Requires zlib ([http://www.zlib.org](http://www.zlib.org)).

To run the automatic unit test suite you need Catch ([https://github.com/philsquared/Catch](https://github.com/philsquared/Catch))


# Installation (Unix)

This version of the software uses cmake to generate the necessary make files or solutions under MS-Windows.

Once you have cmake installed, you should be able to run the following:

<pre>
  tar xf zipios.tar.gz
  mkdir BUILD
  cd BUILD
  cmake ../zipios
  make
  make install
</pre>

The project comes with a build script (see "dev/build") that can be used to run those steps. It will assume that you do not mind to have your BUILD directory blown away and rebuilds everything. It also may setup various flags on the command line to build the DEBUG version, for example.

If you make changes to the source tree, you may re-run the make from the source tree with something like:

<pre>
  make -C ../BUILD
</pre>

For details about available installation configurations of cmake packages refer to the CMake documentation online [http://www.cmake.org/](http://www.cmake.org/)

By default, 'make install' installs the Zipios++ header files under /usr/include/zipios++/ and the library libzipios.so under /usr/lib/. You can choose another base path than /usr/ using the following option on the 'cmake' command line:

<pre>
  -DCMAKE_INSTALL_PREFIX=/home/alexis/zipios
</pre>

The build script actually installs everything under BUILD/dist so one can verify the results and package them before shipping.

Running make also builds one test program. It can be found in the tests directory in your BUILD folder. It is one program that actually runs many tests.


# Installation (Windows)

CMake comes with a graphical tool one can use under MS-Windows to configure and generate a project supporting cmake. You will find more information about cmake on their official website [http://www.cmake.org/](http://www.cmake.org/)

The output of CMake can be projects and a solution for Visual Studio C++ or a set of nmake files. cmake also supports other formats such as JOM.

Once the cmake output was generated, you can run your build tools and then run the INSTALL target. That will install the binary files in one place.

It is strongly advise that your define the CMAKE_INSTALL_PREFIX variable before you install anything.


# Status and Documentation

Please refer to the online documentation at [http://zipios.sourceforge.net](http://zipios.sourceforge.net)

At this time, we generate the HTML and Latex version of the documentation. It is pretty big, but we'll do our best to offer a .tar.gz of the documentation on SourceForge.net

If you have Doxygen installed, then the documentation will automatically be generated. Note that under MS-Windows you may have to specify a path for cmake to find Doxygen and properly generate the output. The setup makes use of dot to generate images showing relationships between classes and files.


# Bugs

Submit bug reports and patches via
[http://sourceforge.net/projects/zipios](http://sourceforge.net/projects/zipios)
