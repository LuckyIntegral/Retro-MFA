###############################################################################
######                            PROPERTIES                             ######
###############################################################################

CC			= cc
RM			= rm -rf
CFLAGS		= -Wall -Wextra -Werror -MD -MP -g
LINKFLAGS	= -lmlx -lXext -lX11
MAKEFLAGS	= -j$(nproc) --no-print-directory

NAME		= retromfa
SRCSDIR		= srcs
SRCS		= \
			${SRCSDIR}/listUtils.c \
			${SRCSDIR}/fileReader.c \
			${SRCSDIR}/main.c

OBJSDIR		= objs
OBJS		= $(addprefix $(OBJSDIR)/, $(SRCS:.c=.o))
DEPS		= $(addprefix $(OBJSDIR)/, $(SRCS:.c=.d))

###############################################################################
######                               RULES                               ######
###############################################################################

all		: $(NAME)

$(NAME)	: ${OBJS}
		$(CC) $(LINKFLAGS) -o $@ $^

${OBJSDIR}/%.o	: %.c
		@mkdir -p $(dir $@)
		${CC} ${CFLAGS} -c $< -o $@

clean	:
		$(RM) $(OBJSDIR)

fclean	:
		$(RM) $(OBJSDIR) $(NAME)

re		:
		$(RM) $(OBJSDIR) $(NAME)
		$(MAKE) all

run		:
		$(MAKE) re
		./$(NAME) mfa/brown.mfa

test	:
		$(MAKE) re
		valgrind --track-fds=yes --leak-check=full --show-leak-kinds=all ./$(NAME) mfa/brown.mfa

-include $(DEPS)

.PHONY: all clean fclean bonus re test
