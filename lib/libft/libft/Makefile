# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: beaudibe <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/10 08:28:08 by beaudibe          #+#    #+#              #
#    Updated: 2022/11/20 09:45:27 by beaudibe         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS			=	ft_striteri.c ft_putendl_fd.c ft_putnbr_fd.c ft_putstr_fd.c ft_putchar_fd.c ft_strmapi.c ft_itoa.c ft_split.c ft_strjoin.c ft_strtrim.c ft_substr.c ft_calloc.c ft_toupper.c ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c ft_tolower.c ft_atoi.c ft_strncmp.c ft_strnstr.c ft_strchr.c ft_strrchr.c ft_strlcat.c ft_strlcpy.c ft_strlen.c ft_strdup.c ft_memcmp.c ft_memset.c ft_bzero.c ft_memcpy.c ft_memccpy.c ft_memmove.c ft_memchr.c ft_strclen.c ft_putnbr_base.c ft_lstmap.c ft_lstiter.c ft_lstclear.c ft_lstdelone.c ft_lstadd_back.c ft_lstadd_front.c ft_lstsize.c ft_lstlast.c ft_lstnew.c
OBJS			=	$(SRCS:.c=.o)

BONUS			=	ft_lstmap.c ft_lstiter.c ft_lstclear.c ft_lstdelone.c ft_lstadd_back.c ft_lstadd_front.c ft_lstsize.c ft_lstlast.c ft_lstnew.c
BONUS_OBJS		=	$(BONUS:.c=.o)

INCLUDES		=	./includes/
CC				=	gcc
RM				=	rm -f
CFLAGS			=	-Wall -Wextra -Werror -I $(INCLUDES)
NAME			=	libft.a

all				:	$(NAME)

$(NAME)			:	$(OBJS)
					ar rcs $(NAME) $(OBJS)

clean			:
					$(RM) $(OBJS) $(BONUS_OBJS)

fclean			:	clean
					$(RM) $(NAME)

re				:	fclean $(NAME)

.c.o			:
					${CC} ${CFLAGS}  -c $< -o ${<:.c=.o}

bonus			:	$(OBJS) $(BONUS_OBJS)
					ar rcs $(NAME) $(OBJS) $(BONUS_OBJS)

.PHONY			:	all clean fclean re bonus
