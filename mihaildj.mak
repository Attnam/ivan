# Merger of Ivan's High Achievement Information Lists makefile for DJGPP environment

# Copyrights (C) Timo Kiviluoto / IvanDev 2004

AR       = ar rs
CC       = gxx -o
FeLibDIR = FeLib
FeLibOBJ = $(FeLibDIR)/Source/bitmap.o $(FeLibDIR)/Source/colorbit.o $(FeLibDIR)/Source/error.o $(FeLibDIR)/Source/feio.o $(FeLibDIR)/Source/felist.o $(FeLibDIR)/Source/femain.o $(FeLibDIR)/Source/femath.o $(FeLibDIR)/Source/festring.o $(FeLibDIR)/Source/graphics.o $(FeLibDIR)/Source/hscore.o $(FeLibDIR)/Source/save.o $(FeLibDIR)/Source/whandler.o
MIHAILDIR = MIHAIL
MIHAILBIN = MIHAIL.exe
MIHAILOBJ = $(MIHAILDIR)/Source/mihail.o
FLAGS = -DGCC -IInclude -I$(FeLibDIR)/Include -O3 -ffast-math -s -W -Wall -pedantic

all:	$(MIHAILBIN)

$(FeLibOBJ) : %.o : %.cpp
	@echo Compiling $@...
	@$(CC) $@ -c $< $(FLAGS)

$(MIHAILOBJ) : %.o : %.cpp
	@echo Compiling $@...
	@$(CC) $@ -c $< $(FLAGS) -I$(MIHAILDIR)/Include

$(MIHAILBIN) : $(FeLibOBJ) $(MIHAILOBJ)
	@echo Compiling $(MIHAILBIN)...
	@$(CC) $(MIHAILBIN) $(FeLibOBJ) $(MIHAILOBJ) $(FLAGS)
