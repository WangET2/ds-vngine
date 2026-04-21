CC = gcc
CFLAGS = -I src/ -I include/ -I unity/src/

UNITY_SRC = unity/src/unity.c
ENGINE_SRC = source/flags.c source/parser.c source/instructions.c
TEST_SRC = tests/test_flags.c

TEST_OUT = test_runner

.PHONY: test clean

test: $(UNITY_SRC) $(ENGINE_SRC) $(TEST_SRC)
	$(CC) $(CFLAGS) $^ -o $(TEST_OUT) && ./$(TEST_OUT)

clean:
	rm -f $(TEST_OUT)