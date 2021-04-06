
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

----------------------------
In case of trouble, contact me: <fober@grame.fr>
