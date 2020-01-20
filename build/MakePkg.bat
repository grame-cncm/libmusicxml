
@echo off

IF NOT EXIST libmusicxml2.sln (
	echo "Warning, this script must be called from the build folder (e.g. win64)"
	GOTO END
)


cmake ..
cmake --build . --config Release --  /maxcpucount:4
cpack -G NSIS64
move libmusicxml2-*.exe ..

:END
