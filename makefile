# .c files
C_SOURCE=$(wildcard *.c libs/**/*.c)


# Object files
OBJ=$(addprefix dist/obj/, $(C_SOURCE:.c=.o))

ALLEGRO_FLAGS = $(shell pkg-config allegro-5 allegro_font-5 --libs allegro_primitives-5 --cflags)
# Compiler
CC=gcc

# Flags for compiler
CC_FLAGS=-W               \
         -Wall            \
         -pedantic        \
		 -lm              \
		 -O2              \
		 -flto 		      \
		 $(ALLEGRO_FLAGS)

#
# Compilation and linking
#

all: $(OBJ) $(create_folder)
	$(CC) -o dist/tron $^ $(CC_FLAGS)
	rm -rf dist/obj


dist/obj/%.o: %.c
	mkdir -p dist/obj/libs
	mkdir -p dist/obj/libs/board
	mkdir -p dist/obj/libs/utils
	mkdir -p dist/obj/libs/player
	$(CC) -o $@ $< -c $(CC_FLAGS)



clean:
	rm -rf *.o tron libs/*.o dist