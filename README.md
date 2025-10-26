# Running Around Cube

## Run project
For linux, following packages are recomended:
```console
make freeglut3-dev libglew-dev libglew2.2 libmagick++-dev libassimp-dev libglfw3 libglfw3-dev
```

Install MSYS2 (if on windows).

Add MSYS2 user bin path to environmental variables.

Install gcc and make through MSYS2:
```console
pacman -S mingw-w64-x86_64-gcc
```
If you are on linux, you can instead install gcc through your package manager.

Run
```console
make windows
```
or
```console
make linux
```
depending on your operating system. After build succeeds, run
```console
make run
```
to execute.

## Run Unit Tests
Install python.

Install meson and ninja through pip, or your package manager, if you are on linux.

Install needed components through MSYS2:
```console
pacman -S mingw-w64-x86_64-gtest
pacman -S mingw-w64-x86_64-pkg-config
``` 
On linux, install gtest and pkg-config through your package manager.

Add MSYS2 mingw64 bin folder to environmental variables.

In project directory run
```console
meson setup -Db_coverage=true --native-file=llvm.ini out/UT
```

To test run
```console
ninja test -C out/UT
```
