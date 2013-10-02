# Iter Vehemens ad Necem makefile for MinGW environment

# Copyrights (C) Timo Kiviluoto / IvanDev 2002-2004

CC       = g++ -o
FeLibDIR = FeLib
FeLibGCH =
#FeLibGCH = $(FeLibDIR)/Include/allocate.h.gch $(FeLibDIR)/Include/bitmap.h.gch $(FeLibDIR)/Include/config.h.gch $(FeLibDIR)/Include/error.h.gch $(FeLibDIR)/Include/fearray.h.gch $(FeLibDIR)/Include/feio.h.gch $(FeLibDIR)/Include/felibdef.h.gch $(FeLibDIR)/Include/felist.h.gch $(FeLibDIR)/Include/feloops.h.gch $(FeLibDIR)/Include/femath.h.gch $(FeLibDIR)/Include/festring.h.gch $(FeLibDIR)/Include/fetime.h.gch $(FeLibDIR)/Include/graphics.h.gch $(FeLibDIR)/Include/hscore.h.gch $(FeLibDIR)/Include/rawbit.h.gch $(FeLibDIR)/Include/rect.h.gch $(FeLibDIR)/Include/save.h.gch $(FeLibDIR)/Include/v2.h.gch $(FeLibDIR)/Include/whandler.h.gch
FeLibOBJ = $(FeLibDIR)/Source/bitmap.o $(FeLibDIR)/Source/config.o $(FeLibDIR)/Source/error.o $(FeLibDIR)/Source/feio.o $(FeLibDIR)/Source/felist.o $(FeLibDIR)/Source/femain.o $(FeLibDIR)/Source/femath.o $(FeLibDIR)/Source/festring.o $(FeLibDIR)/Source/fetime.o $(FeLibDIR)/Source/graphics.o $(FeLibDIR)/Source/hscore.o $(FeLibDIR)/Source/rawbit.o $(FeLibDIR)/Source/save.o $(FeLibDIR)/Source/whandler.o
IVANDIR  = Main
IVANBIN  = IVAN.exe
IVANGCH =
#IVANGCH = $(IVANDIR)/Include/action.h.gch $(IVANDIR)/Include/actions.h.gch $(IVANDIR)/Include/area.h.gch $(IVANDIR)/Include/bodypart.h.gch $(IVANDIR)/Include/char.h.gch $(IVANDIR)/Include/command.h.gch $(IVANDIR)/Include/cont.h.gch $(IVANDIR)/Include/database.h.gch $(IVANDIR)/Include/dungeon.h.gch $(IVANDIR)/Include/entity.h.gch $(IVANDIR)/Include/fluid.h.gch $(IVANDIR)/Include/game.h.gch $(IVANDIR)/Include/gear.h.gch $(IVANDIR)/Include/god.h.gch $(IVANDIR)/Include/gods.h.gch $(IVANDIR)/Include/human.h.gch $(IVANDIR)/Include/iconf.h.gch $(IVANDIR)/Include/id.h.gch $(IVANDIR)/Include/igraph.h.gch $(IVANDIR)/Include/iloops.h.gch $(IVANDIR)/Include/item.h.gch $(IVANDIR)/Include/level.h.gch $(IVANDIR)/Include/lsquare.h.gch $(IVANDIR)/Include/lterra.h.gch $(IVANDIR)/Include/lterras.h.gch $(IVANDIR)/Include/materia.h.gch $(IVANDIR)/Include/materias.h.gch $(IVANDIR)/Include/message.h.gch $(IVANDIR)/Include/miscitem.h.gch $(IVANDIR)/Include/nonhuman.h.gch $(IVANDIR)/Include/object.h.gch $(IVANDIR)/Include/pool.h.gch $(IVANDIR)/Include/proto.h.gch $(IVANDIR)/Include/rain.h.gch $(IVANDIR)/Include/room.h.gch $(IVANDIR)/Include/rooms.h.gch $(IVANDIR)/Include/script.h.gch $(IVANDIR)/Include/smoke.h.gch $(IVANDIR)/Include/square.h.gch $(IVANDIR)/Include/stack.h.gch $(IVANDIR)/Include/team.h.gch $(IVANDIR)/Include/terra.h.gch $(IVANDIR)/Include/trap.h.gch $(IVANDIR)/Include/traps.h.gch $(IVANDIR)/Include/worldmap.h.gch $(IVANDIR)/Include/wskill.h.gch $(IVANDIR)/Include/wterra.h.gch $(IVANDIR)/Include/wterras.h.gch
IVANOBJ  = $(IVANDIR)/Source/actset.o $(IVANDIR)/Source/areaset.o $(IVANDIR)/Source/charset.o $(IVANDIR)/Source/charsset.o $(IVANDIR)/Source/command.o $(IVANDIR)/Source/coreset.o $(IVANDIR)/Source/dataset.o $(IVANDIR)/Source/dungeon.o $(IVANDIR)/Source/game.o $(IVANDIR)/Source/godset.o $(IVANDIR)/Source/iconf.o $(IVANDIR)/Source/id.o $(IVANDIR)/Source/igraph.o $(IVANDIR)/Source/itemset.o $(IVANDIR)/Source/levelset.o $(IVANDIR)/Source/main.o $(IVANDIR)/Source/materset.o $(IVANDIR)/Source/message.o $(IVANDIR)/Source/object.o $(IVANDIR)/Source/roomset.o $(IVANDIR)/Source/script.o $(IVANDIR)/Source/slotset.o $(IVANDIR)/Source/trapset.o $(IVANDIR)/Source/wmapset.o $(IVANDIR)/Source/wskill.o
#FLAGS = -DGCC -DUSE_SDL -DWIZARD -IInclude -I$(FeLibDIR)/Include -pg -ffast-math -W -Wall -pedantic -march=athlon-xp -mrtd -maccumulate-outgoing-args -mwindows
#FLAGS = -DGCC -DUSE_SDL -DWIZARD -IInclude -I$(FeLibDIR)/Include -pg -O3 -ffast-math -march=athlon-xp -mwindows
FLAGS = -DGCC -DUSE_SDL -DWIZARD -IInclude -I$(FeLibDIR)/Include -O3 -ffast-math -s -W -Wall -pedantic -mwindows
LIBS =  -lmingw32 -lSDLmain -lSDL

all:	$(IVANBIN)

$(FeLibGCH) : %.h.gch : %.h
	@echo Compiling $@...
	@$(CC) $@ -c $< $(FLAGS)

$(FeLibOBJ) : %.o : %.cpp
	@echo Compiling $@...
	@$(CC) $@ -c $< $(FLAGS)

$(IVANGCH) : %.h.gch : %.h
	@echo Compiling $@...
	@$(CC) $@ -c $< $(FLAGS) -I$(IVANDIR)/Include

$(IVANOBJ) : %.o : %.cpp
	@echo Compiling $@...
	@$(CC) $@ -c $< $(FLAGS) -I$(IVANDIR)/Include

$(IVANBIN) : $(FeLibGCH) $(FeLibOBJ) $(IVANGCH) $(IVANOBJ)
	@echo Compiling $(IVANBIN)...
	@$(CC) $(IVANBIN) $(FeLibOBJ) $(IVANOBJ) $(FLAGS) $(LIBS)
