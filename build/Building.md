
Compiling the libmusicxml project
======================================================================

----------------------------------------------------------------------
### Compiling the libmusicxml library
----------------------------------------------------------------------
The **libmusicxml** library relies on [CMake](http://www.cmake.org/), a cross-platform, open-source build system. The `build` folder contains the project description and is used to generate native projects.

You should run `make` from the `build` folder. Your target platform is inferred from the Makefile. It builds the libmusicxml library and various sample applications and tools.

Run `make help` for more details and a description of all the possible targets.

##### Note for Android:
The Android SDK and NDK must be installed and `ndk-build`must be available from your current PATH.
- from the build folder: run `make android`
- from the platforms/android folder: run `make`or `make help` for details about the targets.

##### Note for JavaScript:
You must have the [emscripten](http://emscripten.org) compiler installed  and `emcc`must be available from your current PATH.
- from the build folder: run `make js`
- from the javascript/build folder: run `make`

##### Note for Linux platforms:
The procedure to compile is close to the usual 'configure' 'make' 'make install' steps. Actually, you can simply do the following:
    > cd /your_path_to_the_guidolib_project/build
    > make
    > sudo make install

##### Note for Windows platforms:
The CMake project description is "Visual Studio" oriented. Using MingW may require some adaptation of the CMakeLists.txt file. You can benefit of the standard command line tools (make) by installing [MSYS](http://www.mingw.org/wiki/MSYS) of [MSYS2](http://www.msys2.org/).

----------------------------
In case of trouble, contact me: <fober@grame.fr>
