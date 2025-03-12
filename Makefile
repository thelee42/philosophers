
# CC = cc
CC = gcc
CFLAGS = -Wall -Werror -Wextra -g3 -pthread

SRC_DIR = src
OBJ_DIR = objet
INC_DIR = include

INCLUDES = -I$(SRC_DIR) -I$(INC_DIR)

SOURCE = $(wildcard $(SRC_DIR)/*.c)
OBJET = $(addprefix $(OBJ_DIR)/, $(notdir $(SOURCE:.c=.o)))

NAME = philo

GREEN=\033[32m
RESET=\033[0m

all : $(NAME)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(NAME): $(OBJ_DIR) $(OBJET)
	@$(CC) $(CFLAGS) $(OBJET) -o $(NAME)
	@echo "$(GREEN)✅Compliation finished ! All philosophers are on the table$(RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re lclean
