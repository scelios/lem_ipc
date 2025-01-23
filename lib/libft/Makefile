# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: beaudibe <beaudibe@student.42nice.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/17 19:35:43 by beaudibe          #+#    #+#              #
#    Updated: 2022/12/08 21:22:22 by beaudibe         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libftprintf.a

# directories
OBJ_DIR = obj/
SRC_DIR = srcs/
INC_DIR = includes/
LIBFT_DIR = libft/
TEST_DIR = test/

# complier
CFLAGS = -Wall -Werror -Wextra
DEBUG = -Wall -Werror -Wextra

# src & obj files
SRC_FILES = ft_printf.c ft_putchar_printf.c ft_putnbr_base_printf.c ft_putnbr_printf.c ft_putstr_printf.c
OBJ_FILES = $(SRC_FILES:.c=.o)
SRC = $(addprefix $(SRC_DIR), $(SRC_FILES))
OBJ = $(addprefix $(OBJ_DIR), $(OBJ_FILES))
TEST = a.out

# libraries
LIBFT = ft

INCLUDES = -I $(LIBFT_DIR)includes/ -I includes/

all: $(NAME)

$(NAME): $(OBJ)
	@$(MAKE) -C $(LIBFT_DIR)
	@cp $(LIBFT_DIR)/libft.a ./$(NAME)
	@ar rcs $(NAME) $(OBJ)

$(OBJ): $(SRC)
	@mkdir -p $(OBJ_DIR)
	@gcc $(CFLAGS) $(INCLUDES) -c $^
	@mv -f *.o $(OBJ_DIR)

clean:
	@make -C $(LIBFT_DIR) clean
	@/bin/rm -rf $(OBJ_DIR)

fclean: clean
	@make -C $(LIBFT_DIR) fclean
	@/bin/rm -f $(NAME)
	@/bin/rm -f $(TEST)

test: all
	gcc -w test/*.c -L . -l ftprintf $(INCLUDES) -o $(TEST) && ./$(TEST) && rm -rf $(TEST)

re: fclean all

.PHONY: all clean fclean re $(NAME)
