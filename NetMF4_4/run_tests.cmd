call setenv_vs.cmd 15
@echo on
BuildOutput\public\Release\Test\Server\dll\RunTests.exe -testlist %1 -transport Emulator -device Microsoft
