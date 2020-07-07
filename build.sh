pwd
mkdir -p build
cd build
/usr/bin/cmake --version
# cmake ${CMAKE_OPTIONS} -DCMAKE_CXX_FLAGS=${CXX_FLAGS} -DCMAKE_BUILD_TYPE=Coverage -DBUILD_TESTS=ON ..
/usr/bin/cmake ${CMAKE_OPTIONS} -DCMAKE_CXX_FLAGS=${CXX_FLAGS} -DCMAKE_BUILD_TYPE=Release ..
make
# make frank_coverage
# ctest
