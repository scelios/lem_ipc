NAME	:= lemipc
CFLAGS	:= -Wextra -Wall -Werror -Ofast -g -DDEBUG=1
IAFLAGS := -Wextra -Wall -Werror -Ofast -DIA=1
LIBMLX	:= ./lib/MLX42
INCDIR = includes
LGLFW_PATH := /usr/lib/x86_64-linux-gnu/libglfw.so
# LGLFW_PATH := $(shell brew --prefix glfw)
HEADERS	:= -I ../include -I $(LIBMLX)/include
LIBS	:= $(LIBMLX)/build/libmlx42.a ./lib/libft/libftprintf.a -ldl -lglfw -pthread -lm -L $(LGLFW_PATH)/lib/

SRCS	= 	./src/main.c \
					./src/ressources.c \
					./src/graphics.c \
					./src/game.c \
					./src/message.c \
					./src/signal.c 

# SRCS	= 	./src/example.c 

OBJS	= ${SRCS:.c=.o}
LIBFT	= ./lib/libft

all: libmlx $(NAME)

libmlx:
#	@cmake $(LIBMLX) -B $(LIBMLX)/build && make -C $(LIBMLX)/build -j4

%.o: %.c
	@$(CC) $(CFLAGS) -o $@ -c $< -I$(INCDIR) && printf "Compiling: $(notdir $<)\n"

$(NAME): $(OBJS)
	@make -C ${LIBFT}
	@$(CC) $(OBJS) $(LIBS) $(HEADERS) -o $(NAME) && printf "Linking: $(NAME)\n"

# ia: CFLAGS = $(IAFLAGS)
# ia: fclean $(OBJS)
# 	@make -C ${LIBFT}
# 	$(CC) $(OBJS) $(LIBS) $(HEADERS) $(CFLAGS) -o $(NAME) && printf "Linking: $(NAME)\n"

clean:
	@make -C $(LIBFT) clean
	@rm -f $(OBJS)
	@printf "Cleaned object files.\n"

fclean: clean
	@make -C $(LIBFT) fclean
	@rm -f $(NAME)
	@printf "Removed executable: $(NAME).\n"

re: fclean all

git: fclean
	git add *
	git commit -m "auto commit"
	git push

brew:
	brew install glfw

.PHONY: all clean fclean re libmlx git brew