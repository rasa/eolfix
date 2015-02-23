# Define DATE to a date in YYYYMMDD format
# (e.g. 20021231) if you're building a CVS snapshot
# Use %nil if you are building a released version.
%define DATE %nil

Summary: End-of-line character fix utility.
Name: eolfix
Version: 0.2.0
%if "%DATE" == ""
Release: 1
Source: %{name}-%{version}.tar.bz2
%else
Release: 0.cvs%DATE.1
Source: %{name}-%DATE.tar.bz2
%endif
License: MIT
Group: Applications/System
URL: http://www.smithii.com/eolfix/
BuildRoot: %{_tmppath}/%{name}-%{version}-root

%description
Eolfix is a end-of-line character fix utility.

%package -n eolfix
Summary: A command line EOL character fix utility.
Group: Applications/System
#Provides: dos2unix, unix2dos
#Obsoletes: dos2unix, unix2dos

%description -n eolfix
Eolfix will report and optionally correct end-of-line characters in ASCII text files.

%prep
%if "%DATE" == ""
%setup -q
%else
%setup -q -n %name
autogen.sh
%endif
%configure

%build
make

%install
rm -rf "$RPM_BUILD_ROOT"
make install DESTDIR="$RPM_BUILD_ROOT"

%clean
rm -rf $RPM_BUILD_ROOT

%files -n eolfix
%defattr(-,root,root)
%doc CHANGELOG.md NEWS AUTHORS README.md*
%{_bindir}/eolfix
%{_mandir}/man1/eolfix.1*

%changelog
* 2005-12-01 Ross Smith II <ross@smithii.com>
- Initial release
- *: Version 0.1.0
