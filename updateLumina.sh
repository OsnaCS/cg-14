git submodule update --remote
cd lumina
make clean
ASAN=1 make -j2
