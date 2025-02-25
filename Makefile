# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vous                                         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/25 10:00:00 by vous              #+#    #+#              #
#    Updated: 2025/02/25 10:00:00 by vous             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME        = spider

# Compilateur et flags
CXX         = g++
CXXFLAGS    = -Wall -Wextra -Werror -std=c++17 -I./srcs/option/ -I./srcs/

# Inclure libcurl pour HttpRequest
LIBS        = -lcurl

# Chemins des répertoires
SRC_DIR     = srcs
OPT_DIR     = $(SRC_DIR)/option
OBJ_DIR     = objs

# Fichiers sources (ajustez si nécessaire, par ex. si vous avez d’autres .cpp)
SRCS        = $(SRC_DIR)/main.cpp \
              $(SRC_DIR)/HttpRequest.cpp \
              $(SRC_DIR)/WebPage.cpp \
              $(OPT_DIR)/DepthOption.cpp \
              $(OPT_DIR)/OptionRegistry.cpp \
              $(OPT_DIR)/RecursiveOption.cpp \
			  $(OPT_DIR)/PathOption.cpp \
              $(SRC_DIR)/utils.cpp


# Convertir chaque .cpp de SRCS en .o correspondant dans OBJ_DIR
OBJS        = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
OBJS        := $(OBJS:$(OPT_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Règle par défaut
all: $(NAME)

# Création de l'exécutable
$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIBS)
	@echo "Compilation terminée, exécutable généré: $(NAME)"

# Compilation de chaque .cpp vers .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(OPT_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
