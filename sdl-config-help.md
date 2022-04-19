

# do this steps:
1- install sdl2 library from this link:
    https://www.libsdl.org/release/SDL2-devel-2.0.20-mingw.tar.gz


2- copy header files and binary files of the SDL2 library to MinGW folder:
    
    NOTE! VERY IMPORTANT:
        If your MinGW is 32-bit, copy the 32-bit content in the downloaded SDL library (i686-w64-mingw32 folder) to the MinGW folder like the steps below. if not, copy the content in x86_64-w64-mingw32 folder insted like the steps below:

    A-
         Copy the .a files from the appropriate lib directory (in downloaded library) to the MinGW\lib directory(the exact location of which will vary depending on where you installed MinGW).
    B-
        Copy the SDL directory from the appropriate include directory (in downloaded library) to the MinGW\include directory (the exact location of which will vary depending on where you installed MinGW).
    C-
        Copy SDL2.dll from the appropriate bin directory (in downloaded library) to the directory where your executable will be created.
    



Now with command below you can compile your main.c :
    gcc -o main.exe main.c -lmingw32 -lSDL2main -lSDL2


if you were unsuccessful, read this useful link about error handling: 
https://stackoverflow.com/questions/64396979/how-do-i-use-sdl2-in-my-programs-correctly



# some helpful configurations on vscode 
1- add include path to vscode
    add path of MinGW to includePath (for example=>  "C:\\MinGW\\include\\**" )

2- compile and run the code faster!
    insted of running this command (gcc -o main.exe main.c -lmingw32 -lSDL2main -lSDL2
    ) each time, add this to tasks.json file:

        "args": [
        "-o",
        "${fileBasenameNoExtension}.exe",
        "${fileBasenameNoExtension}.c",
        "-lmingw32",
        "-lSDL2main",
        "-lSDL2", 
        "&&", 
        "${fileBasenameNoExtension}.exe",
        ],

    now when we press  Ctrl + Shift + B  , the code compiles and runs :)
