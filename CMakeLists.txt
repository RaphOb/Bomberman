cmake_minimum_required(VERSION 3.13)
project(Bomberman C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake") #chemin pour accéder au fichier cmake
#qui vont trouver les différents fichiers nécessaire pour utiliser SDL2

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror -Wextra") #Affiche TOUS les warnings en mode debug

set(SDL2_PATH "C:\\Program Files\\mingw-w64\\x86_64-8.1.0-posix-seh-rt_v6-rev0\\mingw64\\x86_64-w64-mingw32")
set(SDL2_IMAGE_PATH "C:\\Program Files\\mingw-w64\\x86_64-8.1.0-posix-seh-rt_v6-rev0\\mingw64\\x86_64-w64-mingw32")
set(SDL2_TTF_PATH "C:\\Program Files\\mingw-w64\\x86_64-8.1.0-posix-seh-rt_v6-rev0\\mingw64\\x86_64-w64-mingw32")

find_package(SDL2 REQUIRED)
find_package(SDL2_Image REQUIRED)
find_package(SDL2TTF REQUIRED)

set(SOURCES src/main.c src/SDLEngine.c header/SDLEngine.h src/game.c header/game.h src/map.c header/map.h src/player.c header/player.h src/bit.c header/bit.h src/move.c header/move.h src/renderer.c header/renderer.h src/menu.c header/menu.h src/input.c header/input.h header/serv.h header/client.h src/client.c src/server.c header/reseau.h src/bonus.c header/bonus.h src/bomb.c header/bomb.h src/sound.c header/sound.h)

add_executable(Bomberman ${SOURCES})

include_directories(${SDL2_INCLUDE_DIR} ${SDL2TTF_INCLUDE_DIR}) #inclusion des dossier nécessaire pour le
#développement avec SDL2
target_link_libraries(Bomberman ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL2TTF_LIBRARY} -lSDL2main -lSDL2 -lwsock32) #inclusion des bibliothèques nécessaire pour
#le développement avec SDL2