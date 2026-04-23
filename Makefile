CC = gcc
CFLAGS = -I src/ -I include/ -I unity/src/ -fsanitize=address

UNITY_SRC = unity/src/unity.c
ENGINE_SRC = src/flags.c src/parser.c src/instructions.c
TEST_SRC = tests/test_flags.c tests/test_parser.c tests/main.c

TEST_OUT = test_runner

.PHONY: test clean

test: $(UNITY_SRC) $(ENGINE_SRC) $(TEST_SRC)
	$(CC) $(CFLAGS) $^ -o $(TEST_OUT) && ./$(TEST_OUT)

clean:
	rm -f $(TEST_OUT)