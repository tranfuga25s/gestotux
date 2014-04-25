#/bin/sh
cd ../gestotux-build-desktop/bin
valgrind --gen-suppressions=yes --tool=memcheck --track-origins=yes gestotux
cd ..
cd ..
cd gestotux