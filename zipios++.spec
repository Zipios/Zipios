%define version 0.1.5
%define release	1

Name: zipios++
Summary: Zipios++ is a small C++ library for reading zip files.
Version: %{version}
Release: %{release}
Source: %{name}-%{version}.tar.gz
Group: Development/Libraries
URL: http://zipios.freshmeat.net
BuildRoot: %{_tmppath}/%{name}-%{version}-buildroot
License: GNU LGPL
Prefix: %{prefix}
Packager: "Rui Miguel Seabra" <rms@1407.org>
Distribution: RedHat 7.x Contrib

%description
Zipios++ is a small C++ library for reading zip files. The structure
and public interface are based (somewhat loosely) on the java.util.zip
package. The streams created to access the individual entries in a zip
file are based on the standard iostream library.

Zipios++ also provides a way for an application to support files from
multiple sources (e.g. from zip files or from ordinary directories)
transparently.

%package devel
Summary: zipios++ header files
Group: Development/Libraries
Requires: zipios++

%description devel
Header files for zipios++ development.

%prep
%__rm -rf $RPM_BUILD_ROOT
%setup -q -n %{name}-%{version}

%build
./configure --prefix=$RPM_BUILD_ROOT/usr
%__make

%install
umask 022
%__make install

%files
%defattr(-,root,root)
/usr/lib/libzipios.*


%files devel
%defattr(-,root,root)
/usr/include/zipios++/*.h

%changelog
* Sat Mar 17 2002 Rui M. Silva Seabra <rms@1407.org>
- Creation
