SERSRC = ./ser/main.c \
		 ./ser/t_env.c \
		 ./ser/input_ls.c \
		 ./ser/input_cd.c \
		 ./ser/input_get.c \
		 ./ser/input_put.c \
		 ./ser/input.c \
		 ./ser/io.c \
		 ./ser/accept.c \
		 ./ser/t_fd.c
				
CLISRC = ./cli/main.c \
		 ./cli/loop.c \
		 ./cli/ncurses.c \
		 ./cli/prompt.c \
		 ./cli/prompt2.c \
		 ./cli/prompt_read.c \
		 ./cli/input.c \
		 ./cli/io.c \
		 ./cli/t_msg.c \
		 ./cli/view.c

GCC_FLAGS = -Wall -g
CC = clang $(GCC_FLAGS)

SEROBJ = $(SERSRC:.c=.o)
CLIOBJ = $(CLISRC:.c=.o)

LIBFTDIR = ./libft
LIBFT = $(LIBFTDIR)/libft.a

all: client server

$(LIBFT):
	make -C $(LIBFTDIR)

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
	make -C $(LIBFTDIR) fclean

re: fclean all
