all:compile link

compile:
C:\\mingw64\\bin\\g++ -c main.cpp -DSFML_STATIC -I"C:\SFML-2.6.1\include" -o build/main.o
link:
C:\\mingw64\\bin\\g++ build/main.o -o ejecutable -L"C:\SFML-2.6.1\lib" -static -static-libstdc++  -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lfreetype -lwinmm -lgdi32 -mwindows
