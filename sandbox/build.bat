@echo off
cd ../
set working_dir=%cd%
set library_binaries=%working_dir%/library_binaries

cd sandbox
cmake -A x64 -S . -B ./build -DCMAKE_PREFIX_PATH=%library_binaries%
cmake --build ./build --config Release --parallel
pause