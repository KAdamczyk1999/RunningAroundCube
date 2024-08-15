# Running Around Cube

## Run project
1. Install MSYS2.
2. Add MSYS2 user bin path to environmental variables.
3. Install gcc and make through MSYS2.
4. Go to Project bin folder.
5. Run "make run" to quickly compile and immediately run produced .exe file.
6. If it doesn't succeed, try changing MakeFile's name to "makefile".
7. If that does not help compile manually: "make -f makefile" to compile.
8. Then run produced "test.exe" file.

## Run Unit Tests
1. Install python.
2. Install meson and ninja through pip.
3. Install needed components through MSYS2:
    1. mingw-w64-x86_64-clang.
    2. mingw-w64-x86_64-gtest.
    3. mingw-w64-x86_64-pkg-config.
    4. mingw-w64-x86_64-compiler-rt.
4. Add MSYS2 mingw64 bin folder to environmental variables.
5. Install gcovr through pip.
6. In project directory run "meson setup -Db_coverage=true --native-file=llvm.ini out/UT".
7. To test run "ninja test -C out/UT"