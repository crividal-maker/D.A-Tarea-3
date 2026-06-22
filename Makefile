CC      = gcc
CFLAGS  = -Wall -Wextra -Iincs
SRC     = src/main.c src/deportista.c src/generador_id.c src/busqueda.c src/menu.c src/ordenamiento.c src/merge.c src/quicksort.c src/medicion.c src/pd.c src/greedy.c
OBJ     = $(patsubst src/%.c, obj/%.o, $(SRC))
TARGET  = build/sistema

.PHONY: all clean clean-db folders run

all: folders $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

folders:
	mkdir -p obj build db db/quicksort db/busquedas db/merge plots plots/instructivos plots/graficas docs incs src

run: all
	./$(TARGET)

clean:
	rm -rf obj build db

clean-db:
	rm -rf db
