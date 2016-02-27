CC=clang
LIB_DIR=-Llibs
INCLUDE_DIR=-Iinclude
LDFLAGS=-lglfw3 -lopengl32 -lgdi32
RL_FLAGS=-Wall -Wextra -Wsign-conversion -Wfloat-conversion -Wmissing-prototypes -pedantic -std=c99
DB_FLAGS=-Wall -Wextra -Wsign-conversion -Wfloat-conversion -Wmissing-prototypes -pedantic -std=c99 -g
DOUBLE_DEBAG_FLAGS=-Weverything -std=c99 -g
SOURCES=Basement.c gl3w.c Shaders.c Inf_float_arr_type.c Inf_uint_arr_type.c My_math_utils.c Infinite_str_type.c Load_utils.c View_matrix_utils.c Meshes.c Physics.c
EXECUTABLE=Game_1.exe

all: debug

r: release
d: debug
dd: double_debug

release: $(SOURCES)
	$(CC) $(RL_FLAGS) $(LIB_DIR) $(INCLUDE_DIR) $(SOURCES) $(LDFLAGS) -o $(EXECUTABLE)

debug: $(SOURCES)
	$(CC) $(DB_FLAGS) $(LIB_DIR) $(INCLUDE_DIR) $(SOURCES) $(LDFLAGS) -o $(EXECUTABLE)

double_debug: $(SOURCES)
	$(CC) $(DOUBLE_DEBAG_FLAGS) $(LIB_DIR) $(INCLUDE_DIR) $(SOURCES) $(LDFLAGS) -o $(EXECUTABLE)

clean:
	del *.o
