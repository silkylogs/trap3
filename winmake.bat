@echo off

set exeName=trap3.exe
set flags=-O2 -Wall
set files=PseudocodeSynIdConv.cpp VariableId.cpp FileUtils.cpp main.cpp

@echo on
g++ %flags% -o %exeName% %files%
@echo off

set /p dummyVar=Press enter to continue
