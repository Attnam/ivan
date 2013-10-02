# Ivan Graphics editOR makefile for DJGPP environment

# Copyrights (C) Timo Kiviluoto / IvanDev 2003-2004

AR       = ar rs
CC       = gxx -o
FeLibDIR = FeLib
FeLibOBJ = $(FeLibDIR)/Source/bitmap.o $(FeLibDIR)/Source/colorbit.o $(FeLibDIR)/Source/error.o $(FeLibDIR)/Source/feio.o $(FeLibDIR)/Source/felist.o $(FeLibDIR)/Source/femain.o $(FeLibDIR)/Source/femath.o $(FeLibDIR)/Source/festring.o $(FeLibDIR)/Source/graphics.o $(FeLibDIR)/Source/hscore.o $(FeLibDIR)/Source/save.o $(FeLibDIR)/Source/whandler.o
IGORDIR  = Igor
IGORBIN  = IGOR.exe
IGOROBJ  = $(IGORDIR)/Source/igor.o
FLAGS = -DGCC -IInclude -I$(FeLibDIR)/Include -O3 -ffast-math -s -W -Wall -pedantic

all:	$(IGORBIN)

$(FeLibOBJ) : %.o : %.cpp
	@echo Compiling $@...
	@$(CC) $@ -c $< $(FLAGS)

$(IGOROBJ) : %.o : %.cpp
	@echo Compiling $@...
	@$(CC) $@ -c $< $(FLAGS) -I$(IGORDIR)/Include

$(IGORBIN) : $(FeLibOBJ) $(IGOROBJ)
	@echo Compiling $(IGORBIN)...
	@$(CC) $(IGORBIN) $(FeLibOBJ) $(IGOROBJ) $(FLAGS)
