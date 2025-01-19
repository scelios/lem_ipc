#  |  |  ___ \    \  |         |
#  |  |     ) |  |\/ |   _  |  |  /   _ 
# ___ __|  __/   |   |  (   |    <    __/ 
#    _|  _____| _|  _| \__,_| _|\_\ \___|
#                              by jcluzet
################################################################################
#                                     CONFIG                                   #
################################################################################

NAME        := lemipc
CC        := gcc
#FLAGS    := -Wall -Wextra -Werror -pthread
FLAGS    := -pthread
################################################################################
#                                 PROGRAM'S SRCS                               #
################################################################################

# SRCS        :=      ./src/test.c 
SRCS        :=      ./src/main.c \
					./src/ressources.c \

                          
OBJS        := $(SRCS:.c=.o)

.c.o:
	${CC} ${FLAGS} -c $< -o ${<:.c=.o}

################################################################################
#                                  Makefile  objs                              #
################################################################################


CLR_RMV		:= \033[0m
RED		    := \033[1;31m
GREEN		:= \033[1;32m
YELLOW		:= \033[1;33m
BLUE		:= \033[1;34m
CYAN 		:= \033[1;36m
RM		    := rm -f

LIBFT_DIR   := ./libft
LIBFT       := $(LIBFT_DIR)/libftprintf.a
INCLUDES    := -I $(LIBFT_DIR)/includes
OBJ_DIR	 	:= objs/

${NAME}:	${OBJS}
#			@$(MAKE) -C $(LIBFT_DIR)
			@echo "$(GREEN)Compilation ${CLR_RMV}of ${YELLOW}$(NAME) ${CLR_RMV}..."
			${CC} ${FLAGS} -o ${NAME} ${OBJS} ${LIBFT} ${INCLUDES}
			@echo "$(GREEN)$(NAME) created[0m ✔️"

all:		${NAME}


bonus:		all

clean:
#			@make -C $(LIBFT_DIR) clean
			@ ${RM} *.o */*.o */*/*.o
			@ echo "$(RED)Deleting $(CYAN)$(NAME) $(CLR_RMV)objs ✔️"

fclean:		clean
#			@make -C $(LIBFT_DIR) fclean
			@ ${RM} ${NAME}
			@ echo "$(RED)Deleting $(CYAN)$(NAME) $(CLR_RMV)binary ✔️"

re:			fclean all

.PHONY:		all clean fclean re


