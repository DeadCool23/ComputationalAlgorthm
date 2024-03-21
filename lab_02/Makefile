CC = g++
DYNLIBS = 
INC = -I inc -I classes
CPPFLAGS = -std=c++20 -Wall -Werror -Wextra -Wvla

SRCDIR = src
SRC = $(wildcard $(SRCDIR)/*.cpp)

OBJDIR = ./out
OBJ = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRC))

TARGET = app.exe

.PHONY: all
all: clean
all: $(TARGET)

.PHONY: build
build: $(TARGET)

.PHONY: clean
clean:
	$(RM) $(TARGET) -r $(OBJDIR) 

.PHONY: debug
debug: CPPFLAGS += -O0
debug: CPPFLAGS += -g
debug: $(TARGET)

.PHONY: release
release: CPPFLAGS += -O3
release: $(TARGET)

$(OBJDIR):
	mkdir -p $@

$(TARGET): $(OBJDIR) $(OBJ)
	$(CC) $(OBJ) -o $@ $(CPPFLAGS) $(DYNLIBS) $(INC)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) -c $< -o $@ $(CPPFLAGS) $(DYNLIBS) $(INC)

$(OBJDIR)/%.o: $(TASKSDIR)/%.cpp
	$(CC) -c $< -o $@ $(CPPFLAGS) $(DYNLIBS) $(INC)
