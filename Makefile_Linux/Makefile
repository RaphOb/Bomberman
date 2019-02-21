##
## ETNA PROJECT, modif 21/02/2019 by bukin_g
## Bomberman
## File description:
##      Makefile Bomberman
##

CC = 		gcc
CFLAGS =	-Wextra -Wall -std=gnu99
LIB = 		-lSDL2-2.0 -lSDL2_image-2.0 -lSDL2_ttf -lm -lpthread
NAME =		exec/bomberman
RM =		rm -f
DIR = 		resources
SRC = 		\
		src/bit.c \
		src/map.c \
		src/player.c \
		src/SDLEngine.c \
		src/client.c \
		src/input.c \
		src/menu.c \
		src/move.c \
		src/renderer.c \
		src/game.c \
		src/server.c \
		src/main.c \

OBJ = 		$(SRC:.c=.o)

all : $(NAME)

$(NAME) : $(OBJ)
	$(CC) $(OBJ) $(LIB) -o $(NAME)

clean :
	$(RM) $(OBJ)

fclean : clean
	$(RM) $(NAME)

re : fclean all

compress:
	tar -jcvf $(DIR).tar.bz2 $(DIR)
	$(RM) -r $(DIR)

uncompress:
	tar -xjvf $(DIR).tar.bz2
	$(RM) $(DIR).tar.bz2

.PHONY : all $(NAME) clean fclean re compress uncompress
