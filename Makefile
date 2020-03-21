.PHONY: all clean

CC ?= gcc
CXX ?= g++
CFLAGS += -g -I. -Wall -MP -MMD
CXXFLAGS += $(CFLAGS)

CXXBUILD = $(CXX) $(CXXFLAGS) -MF $(patsubst %.cpp,dep/%.d,$<) -c -o $@ $<

EXE := atlantis.exe

OBJ := alist.o aregion.o ARMGen.o army.o astring.o battle.o \
CFractalMap.o export.o extra.o faction.o fileio.o game.o gamedata.o \
gamedefs.o gameio.o genrules.o items.o main.o market.o modify.o monsters.o \
monthorders.o NameGen.o npc.o object.o orders.o parseorders.o production.o \
rolls.o rules.o runorders.o shields.o skills.o skillshows.o specials.o \
spells.o support.o template.o unit.o world.o

# add the .c
ALL_OBJ := $(OBJ) i_rand.o

DEP  := $(addprefix dep/,$(ALL_OBJ:.o=.d))
OBJS := $(addprefix obj/,$(OBJ))
ALL_OBJS := $(addprefix obj/,$(ALL_OBJ))

### targets
all: dep obj $(EXE)

obj:
	@mkdir $@

dep:
	@mkdir $@

-include $(DEP)

clean::
	@rm -f $(ALL_OBJS) $(EXE)

$(OBJS): obj/%.o: %.cpp
	@$(CXXBUILD)

obj/i_rand.o: i_rand.c
	@$(CC) $(CFLAGS) -MF $(patsubst %.c,dep/%.d,$<) -c -o $@ $<

$(EXE): $(ALL_OBJS)
	@$(CXX) -o $@ $^

