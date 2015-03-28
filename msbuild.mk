all:
	MSBuild.exe /nologo eolfix.sln /p:Configuration=Debug
	MSBuild.exe /nologo eolfix.sln /p:Configuration=Release

clean:
	MSBuild.exe /nologo eolfix.sln /p:Configuration=Debug   /t:clean
	MSBuild.exe /nologo eolfix.sln /p:Configuration=Release /t:clean

upgrade:
	devenv eolfix.sln /upgrade
	
.PHONY:	all clean upgrade

