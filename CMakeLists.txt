cmake_minimum_required(VERSION 3.13)
project(Bomberman C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake") #chemin pour accéder au fichier cmake
#qui vont trouver les différents fichiers nécessaire pour utiliser SDL2

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}") #Affiche TOUS les warnings en mode debug

set(SDL2_PATH "C:\\Program Files\\mingw-w64\\x86_64-8.1.0-posix-seh-rt_v6-rev0\\mingw64\\x86_64-w64-mingw32")
set(SDL2_IMAGE_PATH "C:\\Program Files\\mingw-w64\\x86_64-8.1.0-posix-seh-rt_v6-rev0\\mingw64\\x86_64-w64-mingw32")

find_package(SDL2 REQUIRED)
find_package(SDL2_Image REQUIRED)

set(SOURCES src/main.c src/SDLEngine.c header/SDLEngine.h src/game.c header/game.h src/file.c header/file.h)

add_executable(Bomberman ${SOURCES})

include_directories(${SDL2_INCLUDE_DIR}) #inclusion des dossier nécessaire pour le
#développement avec SDL2
target_link_libraries(Bomberman ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} -lSDL2main -lSDL2) #inclusion des bibliothèques nécessaire pour
#le développement avec SDL2