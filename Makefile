# Makefile of project 'seastar'


# This makefile has been written to allow C and C++ files to be 
# added to the project, even if there is only C++ code in our case.


# Generated files (objects and binary) are same for release and 
# debug building mode, thus you need to rebuild all the project 
# when changing mode.


# Target available are :
#   - all       debug or release version (release by default)
#   - dep       dependencies generation
#   - tarball   backup of the whole project in a tarball
#   - clean     cleanup of the project directory


# Commands for the building chain
C:=gcc
CXX:=g++
LD:=g++
RM:=rm
AR:=ar
TAR:=tar


# Variables of project
PROJ_NAME:=seastar
PROJ_DIR:=.
TARBALL:=$(PROJ_DIR)/$(PROJ_NAME).tar.bz2
MAKEFILE:=$(PROJ_DIR)/Makefile
DOXYFILE:=$(PROJ_DIR)/Doxyfile


# Processing of building parameters
ifeq ($(DEBUG), yes)
  FLAGS+=-DDEBUG
  FLAGS+=-O0
  FLAGS+=-g3
  FLAGS+=-ggdb3
else
  FLAGS+=-O3
endif
CFLAGS:=$(FLAGS)
CXXFLAGS:=$(FLAGS)
LD_FLAGS+=-t


# Project subdirectories
SRC:=$(PROJ_DIR)/src
INC:=$(PROJ_DIR)/inc
OBJ:=$(PROJ_DIR)/obj
LIB:=$(PROJ_DIR)/lib
BIN:=$(PROJ_DIR)/bin
DEP:=$(PROJ_DIR)/dep
CFG:=$(PROJ_DIR)/cfg
TST:=$(PROJ_DIR)/tst
DOC:=$(PROJ_DIR)/doc


# Directory for headers include
INCDIR+=-I$(INC)


# Libraries needed at link time
LIBRARIES+=-lc
LIBRARIES+=-lstdc++
LIBRARIES+=-lm
LIBRARIES+=-lrt
LIBRARIES+=-lpthread
LIBRARIES+=-lgcc
#LIBRARIES+=-ldl


# Project modules (compilation unit)
CXXMODULES+=main
CXXMODULES+=object
CXXMODULES+=application
CXXMODULES+=parameters
CXXMODULES+=console
CXXMODULES+=manager
CXXMODULES+=connection
CXXMODULES+=socket
CXXMODULES+=thread
CXXMODULES+=exception
MODULES+=$(CMODULES)
MODULES+=$(CXXMODULES)


# List of object files
OBJECTS:=$(foreach MOD, $(MODULES), $(OBJ)/$(MOD).o)


# Défintion des commandes possibles avec leurs dépendances (cible par défaut : 'all')
.PHONY: dep all doc tarball clean


# Default target
all: $(BIN)/$(PROJ_NAME)


# Binary dependencies
$(BIN)/$(PROJ_NAME): $(OBJECTS)
	$(LD) $(OBJECTS) $(LIBRARIES) -o $@


# Dependencies includes
-include $(foreach MOD, $(MODULES), $(DEP)/$(MOD).d)


# Dependencies generation
dep:
	for MOD in $(CMODULES)   ; do $(C)   -MM $(SRC)/$$MOD.c   $(INCDIR) -MT $(OBJ)/$$MOD.o -MF $(DEP)/$$MOD.d ; done
	for MOD in $(CXXMODULES) ; do $(CXX) -MM $(SRC)/$$MOD.cpp $(INCDIR) -MT $(OBJ)/$$MOD.o -MF $(DEP)/$$MOD.d ; done


# Generic rule for C compilation
$(OBJ)/%.o: $(SRC)/%.c
	$(C) $(CFLAGS) -c $< $(INCDIR) -MMD -MT $(OBJ)/$*.o -MF $(DEP)/$*.d -o $@


# Generic rule for C++ compilation
$(OBJ)/%.o: $(SRC)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< $(INCDIR) -MMD -MT $(OBJ)/$*.o -MF $(DEP)/$*.d -o $@


# Documentation generation
doc:
	doxygen
	$(MAKE) -C $(DOC)/latex


# Backup of all the files in a tarball
tarball:
	$(RM) -f $(TARBALL)
	$(TAR) cvjf $(TARBALL) \
    `basename $(SRC)`    \
    `basename $(INC)`    \
    `basename $(OBJ)`    \
    `basename $(LIB)`    \
    `basename $(BIN)`    \
    `basename $(DEP)`    \
    `basename $(CFG)`    \
    `basename $(DOC)`    \
    $(MAKEFILE)          \
    $(DOXYFILE)


# Cleanup of directory
clean:
	$(RM) -fr $(DEP)/*     \
	          $(OBJ)/*     \
	          $(BIN)/*     \
	          $(DOC)/*     \
	          $(TARBALL)
