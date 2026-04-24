CC = gcc
CFLAGS = -I src/ -I include/ -I unity/src/ -I CMock/src -fsanitize=address

UNITY_SRC = unity/src/unity.c
ENGINE_SRC = src/flags.c src/parser.c src/instructions.c
MOCK_SRC = mocks/Mockdisplay.c mocks/Mockrenderer.c mocks/Mocktext.c
CMOCK_SRC = CMock/src/cmock.c
TEST_SRC = tests/test_flags.c tests/test_parser.c tests/main.c

TEST_OUT = test_runner

.PHONY: test clean

test: $(UNITY_SRC) $(CMOCK_SRC) $(ENGINE_SRC) $(MOCK_SRC) $(TEST_SRC)
	$(CC) $(CFLAGS) $^ -o $(TEST_OUT) && ./$(TEST_OUT)

clean:
	rm -f $(TEST_OUT)