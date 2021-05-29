# ********************************************************* #
#                                                           #
#                                                           #
#    Makefile                                               #
#                                                           #
#    By: Alvaro Romero <alvromero96@gmail.com>              #
#                                                           #
#    Created: 2021/05/29                                    #
#                                                           #
# ********************************************************* #

CC        = g++
CFLAGS    = -Wall -Werror -Wextra -g3
SRC       = wav_generator.cpp
OBJ       = $(addprefix $(OBJDIR)/,$(SRC:.cpp=.o))
DEL       = rm -rf
NAME      = wav-generator
INC       = wav_generator.h
WAV       = *.wav
SRCDIR    = ./src
INCDIR    = ./inc
OBJDIR    = ./obj

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -I $(INCDIR) -o $(NAME)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR) \
	&& $(CC) $(CFLAGS) -I $(INCDIR) -o $@ -c $<

clean:
	$(DEL) $(OBJ) $(OBJDIR)

fclean: clean
	$(DEL) $(NAME)

sclean: fclean
	$(DEL) $(WAV)

re: fclean all

test: all
	@echo "my_test" | ./$(NAME)