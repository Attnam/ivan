# Iter Vehemens ad Necem makefile for DJGPP environment

# Copyrights (C) Timo Kiviluoto / IvanDev 2000-2004

CC       = gxx -o
FeLibDIR = FeLib
FeLibOBJ = $(FeLibDIR)/Source/bitmap.o $(FeLibDIR)/Source/config.o $(FeLibDIR)/Source/error.o $(FeLibDIR)/Source/feio.o $(FeLibDIR)/Source/felist.o $(FeLibDIR)/Source/femain.o $(FeLibDIR)/Source/femath.o $(FeLibDIR)/Source/festring.o $(FeLibDIR)/Source/fetime.o $(FeLibDIR)/Source/graphics.o $(FeLibDIR)/Source/hscore.o $(FeLibDIR)/Source/rawbit.o $(FeLibDIR)/Source/save.o $(FeLibDIR)/Source/whandler.o
IVANDIR  = Main
IVANBIN  = IVAN.exe
IVANOBJ  = $(IVANDIR)/Source/actset.o $(IVANDIR)/Source/areaset.o $(IVANDIR)/Source/charset.o $(IVANDIR)/Source/charsset.o $(IVANDIR)/Source/command.o $(IVANDIR)/Source/coreset.o $(IVANDIR)/Source/dataset.o $(IVANDIR)/Source/dungeon.o $(IVANDIR)/Source/game.o $(IVANDIR)/Source/godset.o $(IVANDIR)/Source/iconf.o $(IVANDIR)/Source/id.o $(IVANDIR)/Source/igraph.o $(IVANDIR)/Source/itemset.o $(IVANDIR)/Source/levelset.o $(IVANDIR)/Source/main.o $(IVANDIR)/Source/materset.o $(IVANDIR)/Source/message.o $(IVANDIR)/Source/object.o $(IVANDIR)/Source/roomset.o $(IVANDIR)/Source/script.o $(IVANDIR)/Source/slotset.o $(IVANDIR)/Source/trapset.o $(IVANDIR)/Source/wmapset.o $(IVANDIR)/Source/wskill.o
FLAGS = -DGCC -DWIZARD -IInclude -I$(FeLibDIR)/Include -O3 -ffast-math -s -W -Wall -pedantic

all:	$(IVANBIN)

$(FeLibOBJ) : %.o : %.cpp
	@echo Compiling $@...
	@$(CC) $@ -c $< $(FLAGS)

$(IVANOBJ) : %.o : %.cpp
	@echo Compiling $@...
	@$(CC) $@ -c $< $(FLAGS) -I$(IVANDIR)/Include

$(IVANBIN) : $(FeLibOBJ) $(IVANOBJ)
	@echo Compiling $(IVANBIN)...
	@$(CC) $(IVANBIN) $(FeLibOBJ) $(IVANOBJ) $(FLAGS)
