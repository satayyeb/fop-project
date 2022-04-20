# Fundamentals Of Programming Project (Island Soldiers)

## Description

- 💻 Implementation of [State.io game](https://play.google.com/store/apps/details?id=io.state.fight&hl=en&gl=US)
- 🎮 A simple strategic game
- 💯 100% written in C

## Download Build

- For Linux download [linux-build.zip](https://github.com/satayyeb/fop-project/blob/main/linux-build.zip)
- For Windows download [windows-64bit-build.zip](https://github.com/satayyeb/fop-project/blob/main/windows-64bit-build.zip)

## Build the project (Linux)
Run these commands:
```
mkdir build1
cd build1
cmake ../
cmake --build .
```

## Build the project (Windows)
- Make sure [*CMake*](https://cmake.org/download/) is installed in your system.
- Run the commands above.
- If build failed, run:
  ```
  mkdir build-test
  cd build-test
  cmake-gui
  ```
- To generate **CMakeCache.txt**, do these steps:
  - Set the working directory. (yello arrows)
  - Click Configure. (red arrow)
  - Select a generator. Do not select Visual Studio.
  - Click Configure again.
  - Click Generate.
- Now, run this:
  ```
  cmake --build .
  ```
- To run the game (exe file) on Windows, you need to copy the dynamic libraries (or dll files) to the "build-test" folder where the exe is located. The dll files are in "./sdl2/necessary dll files" directory. Copy them where the exe file is located.

## CMake-GUI configuration pictures

![alt text](https://s6.uupload.ir/files/f1_3xbo.jpg)
![alt text](https://s6.uupload.ir/files/f2_rf64.jpg)
![alt text](https://s6.uupload.ir/files/f4_5vcn.jpg)

---

## Some pictures of the game
![alt text](https://s6.uupload.ir/files/1_zu.jpg)
![alt text](https://s6.uupload.ir/files/2_8xkn.jpg)
![alt text](https://s6.uupload.ir/files/3_i7dm.jpg)
![alt text](https://s6.uupload.ir/files/4_zbtc.jpg)

---


### enjoy 😀
