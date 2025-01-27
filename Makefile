NAME	:= lemipc
# CFLAGS	:= -Wextra -Wall -Werror -Ofast -g -DDEBUG=1
CFLAGS	:= -Ofast -g -DDEBUG=1

LIBMLX	:= ./lib/MLX42
INCDIR = includes
LGLFW_PATH := $(shell brew --prefix glfw)
HEADERS	:= -I ../include -I $(LIBMLX)/include
LIBS	:= $(LIBMLX)/build/libmlx42.a ./lib/libft/libftprintf.a -ldl -lglfw -pthread -lm -L $(LGLFW_PATH)/lib/

SRCS	= 	./src/main.c \
					./src/ressources.c \
					./src/graphics.c \
					./src/game.c \

OBJS	= ${SRCS:.c=.o}
LIBFT	= ./lib/libft

all: libmlx $(NAME)


libmlx:
#	 @cmake $(LIBMLX) -B $(LIBMLX)/build && make -C $(LIBMLX)/build -j4

%.o: %.c
	@$(CC) $(CFLAGS) -o $@ -c $< -I$(INCDIR) && printf "Compiling: $(notdir $<)"

$(NAME): $(OBJS)
	make -C ${LIBFT}
	@$(CC) $(OBJS) $(LIBS) $(HEADERS) -o $(NAME)

bonus: libmlx $(OBJS)
	make -C ${LIBFT}
	@$(CC) $(OBJS) $(LIBS) $(HEADERS) -o miniRT_bonus

clean:
#	@make -C $(LIBFT) clean
	@rm -f $(OBJS)


fclean: clean
#	@make -C $(LIBFT) fclean
	@rm -f $(NAME)
	@rm -f $(NAME)_bonus

git: fclean
	git add *
	git commit -m "auto commit"
	git push

brew:
	brew install glfw

re: clean all

.PHONY: all, clean, fclean, re, libmlx, git, brew, bonus