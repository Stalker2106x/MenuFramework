##
## Makefile for MenuFramework
##
## Made by Maxime Martens
##

SRCDIR = src

BINDIR = bin

LIBDIR = thirdparty

##

CXX = g++

RM = rm -rf

CXXFLAGS	= -std=c++14 -ggdb -W -Wall -Wpedantic -Iinclude
CXXFLAGS += -I$(LIBDIR) -I$(LIBDIR)/json/include/ -I$(LIBDIR)/pugixml/src/ -I$(LIBDIR)/sol2/ -I$(LIBDIR)/lua/

LDFLAGS		= -L$(LIBDIR)/lua/ -llua -shared
# -static-libgcc

ifeq ($(OS),Windows_NT)
	MAKE = mingw32-make
	NAME = $(BINDIR)/MenuFramework.dll
else
	MAKE = make
	NAME = $(BINDIR)/MenuFramework.so
endif

SRCS = 	$(SRCDIR)/Localization.cpp					\
				$(SRCDIR)/GraphicsRenderer.cpp			\
				$(SRCDIR)/Menu.cpp									\
				$(SRCDIR)/MenuItem.cpp							\
				$(SRCDIR)/ScriptEngine.cpp					\
				$(SRCDIR)/DataFile.cpp							\
				$(LIBDIR)/pugixml/src/pugixml.cpp

OBJS = $(SRCS:.cpp=.o)

#Rules

all: library

deps:
ifeq ($(OS),Windows_NT)
	-cd $(LIBDIR)/lua/ && $(MAKE) mingw
else
	-cd $(LIBDIR)/lua/ && $(MAKE) linux
endif

library:	$(info ****************** Run "make deps" before compiling library *****************) $(OBJS)
	$(CXX) $(OBJS) $(RES) -o $(NAME) $(LDFLAGS)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

cleandeps:
ifeq ($(OS),Windows_NT)
	-cd $(LIBDIR)/lua/ && $(MAKE) clean
else
	-cd $(LIBDIR)/lua/ && $(MAKE) clean
endif

re: fclean all

.PHONY: all clean fclean re
