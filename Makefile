APP = facebook_grapher

BIN_DIR = bin
SRC_DIR = src
OBJ_DIR = Obj

APP := $(BIN_DIR)/$(APP)

APP_SRCS = CCamera.cpp Core/CHash.cpp Core/CMath.cpp Core/CTimer.cpp \
		   Gfx/CFont.cpp Gfx/GfxHelper.cpp Graph/CGrapher.cpp \
		   Graph/Graph.cpp main.cpp Resources/CTexture.cpp

INCLUDE_DIR = -I $(SRC_DIR)
CPPFLAGS = -g -Wall -Wno-reorder $(INCLUDE_DIR)
LIBS = -lSDL -lSDL_image -lGL -lGLU

APP_OBJS = $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(APP_SRCS))
APP_DEPS = $(patsubst %.cpp,$(OBJ_DIR)/%.d,$(APP_SRCS))

all: dirs $(APP)

dirs: $(BIN_DIR) $(OBJ_DIR)

$(BIN_DIR) $(OBJ_DIR):
	@mkdir -p $@

$(APP): $(APP_OBJS)
	$(CXX) $(CPPFLAGS) -o $(APP) $(APP_OBJS) $(LIBS)

$(APP_DEPS) : $(OBJ_DIR)/%.d : $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) -MM $(CPPFLAGS) $< > $@

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) -c $< -o $@

.PHONY : all clean dirs

clean:
	rm -rf $(BIN_DIR)/$(APP) Obj

ifneq "$(MAKECMDGOALS)" "clean"
	-include $(APP_DEPS)
endif
