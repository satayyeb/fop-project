This article was written by me.

I wrote this article so as not to forget the important points.

# Useful link:
read this useful link about error handling: 
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

3- for build the project I suggest using Cmake. it's absolutely better.
