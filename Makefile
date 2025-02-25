###################################
# Noms des exécutables
###################################
SPIDER_NAME       = spider
SCORPION_NAME     = scorpion

###################################
# Compilateur et flags
###################################
CXX               = g++
CXXFLAGS          = -Wall -Wextra -Werror -std=c++17 -I./srcs/option/ -I./srcs/

# Liens (bibliothèques)
LIBS              = -lcurl

###################################
# Répertoires sources/objets
###################################
SRC_DIR           = srcs
OPT_DIR           = $(SRC_DIR)/option
OBJ_DIR           = objs

###################################
# Sources du programme spider
###################################
SPIDER_SRCS       = $(SRC_DIR)/main.cpp \
                    $(SRC_DIR)/HttpRequest.cpp \
                    $(SRC_DIR)/WebPage.cpp \
                    $(OPT_DIR)/DepthOption.cpp \
                    $(OPT_DIR)/OptionRegistry.cpp \
                    $(OPT_DIR)/RecursiveOption.cpp \
                    $(OPT_DIR)/PathOption.cpp \
                    $(SRC_DIR)/utils.cpp

# Objets spider (transformation .cpp -> .o dans OBJ_DIR)
SPIDER_OBJS       = $(SPIDER_SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
SPIDER_OBJS       := $(SPIDER_OBJS:$(OPT_DIR)/%.cpp=$(OBJ_DIR)/%.o)

###################################
# Sources du programme scorpion
###################################
# On suppose que scorpion.cpp est directement à la racine (voir ton tree).
SCORPION_SRCS     = scorpion.cpp

# Objets scorpion
SCORPION_OBJS     = $(SCORPION_SRCS:%.cpp=$(OBJ_DIR)/%.o)

###################################
# Règle "all": compiler spider + scorpion
###################################
all: $(SPIDER_NAME) $(SCORPION_NAME)

###################################
# Cible spider
###################################
$(SPIDER_NAME): $(SPIDER_OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIBS)
	@echo "Compilation terminée: $(SPIDER_NAME)"

###################################
# Cible scorpion
###################################
$(SCORPION_NAME): $(SCORPION_OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@  -lexiv2
	@echo "Compilation terminée: $(SCORPION_NAME)"

###################################
# Compilation des .cpp -> .o
###################################
# 1) Règle générique pour les sources dans srcs/
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 2) Règle générique pour les sources dans srcs/option/
$(OBJ_DIR)/%.o: $(OPT_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 3) Règle pour scorpion.cpp (puisque c'est à la racine)
$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

###################################
# Nettoyage
###################################
clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(SPIDER_NAME) $(SCORPION_NAME)

re: fclean all

.PHONY: all clean fclean re
