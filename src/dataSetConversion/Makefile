# Compiler and flags
CXX      = g++
CXXFLAGS = -Wall -Wextra -Werror -std=c++17
GTEST_CXXFLAGS = -lgtest -lgtest_main -pthread

# Files
NAME     = converter
SRCS     = main.cpp convert.cpp
OBJS     = $(SRCS:.cpp=.o)

# Tests
TEST_NAME = test_converter
TEST_SRCS = tests/parseDatasetTest.cpp convert.cpp
TEST_OBJS = $(TEST_SRCS:.cpp=.o)

# Rules
all: $(NAME) $(TEST_NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

tests: $(TEST_NAME)
	./$(TEST_NAME)

$(TEST_NAME): $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) $(TEST_OBJS) -o $(TEST_NAME) $(GTEST_CXXFLAGS)

clean:
	rm -f $(OBJS) $(TEST_OBJS)

fclean: clean
	rm -f $(NAME) $(TEST_NAME)

re: fclean all

.PHONY: all clean fclean re tests
