SERSRC = ./ser/main.c
CLISRC = ./cli/main.c

GCC_FLAGS = -Wall -Werror -Wextra -g
CC = clang $(GCC_FLAGS)

SEROBJ = $(SERSRC:.c=.o)
CLIOBJ = $(CLISRC:.c=.o)

LIBFTDIR = ./libft
LIBFT = $(LIBFTDIR)/libft.a

all: client server

client: $(CLIOBJ) $(LIBFT)
	$(CC) -o client $(CLIOBJ) -lft -L./libft/ -lncurses

server: $(SEROBJ) $(LIBFT)
	$(CC) -o server $(SEROBJ) -lft -L./libft/ -lncurses

%.o : %.c
	@$(CC) -o $@ -c $^ -I./libft/

clean:
	@$(RM)  $(SEROBJ)
	@$(RM)  $(CLIOBJ)

fclean: clean
	@$(RM) client
	@$(RM) server

re: fclean all