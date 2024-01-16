NAME = 			ft_ping

LIBFT = ./libft/libft.a

SRC_FILES 			= main.c 

SRCS = 			${addprefix srcs/, ${SRC_FILES}}
OBJS = 			${SRCS:.c=.o}
HEADERS = 		includes/main.h

CMD = 			gcc
FLAGS = 		-Wall -Werror -Wextra

LIB =			./libft/libft.a

all: 			library $(NAME)

$(NAME): 		$(OBJS) $(LIB) Makefile
				$(CMD) $(FLAGS) $(OBJS) $(LIB) -o $(NAME) $(FLAGSX)
				
%.o:			%.c $(HEADERS) $(LIB)
				$(CMD) $(FLAGS) -c $< -o $@

library: 
					${MAKE} -C ./libft

clean:
		rm -rf $(OBJS)
		${MAKE} clean -C ./libft

fclean: clean
		rm -rf $(NAME)
		${MAKE} fclean -C ./libft
		

re: fclean all

.PHONY: library clean all re fclean