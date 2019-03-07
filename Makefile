##
## Makefile for MenuFramework
##
## Made by Maxime Martens
##

SRCDIR = src

BINDIR = lib

LIBDIR = thirdparty

##

CXX = g++

ARCHIVE = ar rcs

RM = rm -rf

CXXFLAGS	= -std=c++17 -ggdb -W -Wall -Wpedantic -Iinclude -fPIC
CXXFLAGS += -I$(LIBDIR) -I$(LIBDIR)/json/include/ -I$(LIBDIR)/pugixml/src/ -I$(LIBDIR)/sol2/ -I$(LIBDIR)/lua/

LDFLAGS		= $(LIBDIR)/lua/liblua.a
# -static-libgcc

ifeq ($(OS),Windows_NT)
	MAKE = mingw32-make
	NAME = $(BINDIR)/MenuFramework.a
else
	MAKE = make
	NAME = $(BINDIR)/MenuFramework.a
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
	$(ARCHIVE) $(NAME) $(OBJS) $(LDFLAGS)

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
