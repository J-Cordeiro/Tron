# .c files
C_SOURCE=$(wildcard *.c libs/**/*.c)


# Object files
OBJ=$(addprefix dist/obj/, $(C_SOURCE:.c=.o))

ALLEGRO_FLAGS = $(shell pkg-config allegro-5 allegro_audio-5 allegro_acodec-5 allegro_image-5 allegro_font-5 allegro_ttf-5 --libs allegro_primitives-5 --cflags)
# Compiler
CC=gcc

# Flags for compiler
CC_FLAGS=-W               \
         -Wall            \
		 -Wextra          \
         -pedantic        \
		 -lm              \
		 $(ALLEGRO_FLAGS)

#
# Compilation and linking
#
$(shell mkdir -p dist/obj/libs/{board,utils,player,menu})
$(shell cp -r assets dist/)
all: CC_FLAGS +=-O2 -flto
all: $(OBJ)
	$(CC) -o dist/tron $^ $(CC_FLAGS)
	rm -rf dist/obj

debug: CC_FLAGS +=-g3
debug: $(OBJ)
	$(CC) -o dist/tron $^ $(CC_FLAGS) 
	rm -rf dist/obj

dist/obj/%.o: %.c
	$(CC) -o $@ $< -c $(CC_FLAGS)

clean:
	rm -rf *.o tron libs/*.o dist