rm log_compile_common
rm log_compile_core
rm log_compile_unittest_cpp
rm log_unittest_cpp

rm Dev/lib/libace_common.a
rm Dev/lib/libace_core.a

(cd Dev/ace_cpp/gcc/;
rm *.o;
make clean;
make common;
make core;
mv log_compile_common ../../../;
mv log_compile_core ../../../;)

(cd Dev/unitTest_cpp/gcc/;
rm *.o;
make clean;
rm unittest*;
make unittest;
mv log_compile_unittest_cpp ../../../;
./unittest_app >log_unittest_cpp;
mv log_unittest_cpp ../../../;)


