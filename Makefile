CC = gcc
CFLAGS = -I src/ -I include/ -I unity/src/ -fsanitize=address

UNITY_SRC = unity/src/unity.c
ENGINE_SRC = src/flags.c src/parser.c src/instructions.c

TEST_OUT = test_runner

.PHONY: parsertest flagtest clean

flagtest: $(UNITY_SRC) $(ENGINE_SRC) tests/test_flags.c
	$(CC) $(CFLAGS) $^ -o $(TEST_OUT) && ./$(TEST_OUT)

parsertest: $(UNITY_SRC) $(ENGINE_SRC) tests/test_parser.c
	$(CC) $(CFLAGS) $^ -o $(TEST_OUT) && ./$(TEST_OUT)

clean:
	rm -f $(TEST_OUT)