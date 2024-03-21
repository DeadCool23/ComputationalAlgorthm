CC = g++
DYNLIBS = 
INC = -I inc -I classes
CPPFLAGS = -std=c++20 -Wall -Werror -Wpedantic -Wextra -Wvla

SRCDIR = src
SRC = $(wildcard $(SRCDIR)/*.cpp)

OBJDIR = ./out
OBJ = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRC))

APPSDIR = ./apps
TARGET1 = $(APPSDIR)/task1.exe
TARGET2 = $(APPSDIR)/task2.exe
TARGET3 = $(APPSDIR)/task3.exe

TASKSDIR = tasks
SRC1 = $(SRC) $(TASKSDIR)/task1.cpp
SRC2 = $(SRC) $(TASKSDIR)/task2.cpp
SRC3 = $(SRC) $(TASKSDIR)/task3.cpp

OBJ1 = $(OBJ) $(OBJDIR)/task1.o
OBJ2 = $(OBJ) $(OBJDIR)/task2.o
OBJ3 = $(OBJ) $(OBJDIR)/task3.o

.PHONY: all
all: task1 task2 task3

.PHONY: task1
task1: $(TARGET1)

.PHONY: task2
task2: $(TARGET2)

.PHONY: task3
task3: $(TARGET3)

.PHONY: build
build: all

.PHONY: clean
clean:
	$(RM) -r $(OBJDIR) $(APPSDIR)

.PHONY: debug
debug: CPPFLAGS += -O0
debug: CPPFLAGS += -g
debug: all

.PHONY: release
release: CPPFLAGS += -O3
release: all

.PHONY: helper
helper: $(APPSDIR)
helper: helper.cpp
	$(CC) $< -o $(APPSDIR)/helper.exe $(CPPFLAGS) $(DYNLIBS) $(INC)

$(OBJDIR):
	mkdir -p $@

$(APPSDIR):
	mkdir -p $@

$(TARGET1): $(APPSDIR) $(OBJDIR) $(OBJ1)
	$(CC) $(OBJ1) -o $@ $(CPPFLAGS) $(DYNLIBS) $(INC)

$(TARGET2): $(APPSDIR) $(OBJDIR) $(OBJ2)
	$(CC) $(OBJ2) -o $@ $(CPPFLAGS) $(DYNLIBS) $(INC)

$(TARGET3): $(APPSDIR) $(OBJDIR) $(OBJ3)
	$(CC) $(OBJ3) -o $@ $(CPPFLAGS) $(DYNLIBS) $(INC)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) -c $< -o $@ $(CPPFLAGS) $(DYNLIBS) $(INC)

$(OBJDIR)/%.o: $(TASKSDIR)/%.cpp
	$(CC) -c $< -o $@ $(CPPFLAGS) $(DYNLIBS) $(INC)
