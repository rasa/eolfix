all:
	VCBuild.exe /nologo eolfix.vcproj /rebuild

clean:
	VCBuild.exe /nologo eolfix.vcproj /clean
	
upgrade:
	devenv eolfix.sln /upgrade

.PHONY:	all clean upgrade

