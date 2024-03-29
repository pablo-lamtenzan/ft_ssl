NAME		=		ft_ssl
OBJDIR		=		relocs
CC			=		/usr/bin/gcc
RM			=		/bin/rm

include				srcs.mk

CFLAGS		=		-Wall -Wextra -Werror #-g3 -fsanitize=address
IFLAGS		=		-I$(INCDIR)
SFLAGS		=		

OBJS		=		$(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))

all: $(NAME)
	@:

$(NAME) : $(OBJDIR) $(OBJS) $(HDRS)
	@$(CC) -o $(NAME) $(CFLAGS) $(OBJS) $(SFLAGS)
	@echo LINK $@

$(OBJDIR):
	@mkdir -p $@
	@echo MKDIR $@

$(OBJDIR)/%.o : $(SRCDIR)/%.c $(HDRS)
	@mkdir -p $(shell dirname $@)
	@$(CC) -c -o $@ $(CFLAGS) $(IFLAGS) $<
	@echo CC $<

clean:
	@$(RM) -rf $(OBJDIR)
	@echo RM $(OBJDIR)

fclean: clean
	@$(RM) -rf $(NAME)
	@echo RM $(NAME)

re: fclean all
