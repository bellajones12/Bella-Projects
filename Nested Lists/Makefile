CC=gcc
# FLAGS=-Wall -Werror
FLAGS=-Wall -Werror -fsanitize=address -g
OBJS=mtll.o main.o part1.o part2.o part3.o helper.o
TARGET=mtll

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(FLAGS) $^ -o $@

%.o: %.c
	$(CC) -c $(FLAGS) $< -o $@

tests: $(TARGET)
	chmod u+x test.sh
	chmod -R +x /home/comp2017/multi-type-linked-list

.PHONY:
run_tests: tests
	./test.sh 

.PHONY:
clean:
	rm -f $(OBJS)
	rm -f $(TARGET)

run:
	./mtll

build:
	gcc -c -Wall -Werror -fsanitize=address -g mtll.c -o mtll.o
	gcc -c -Wall -Werror -fsanitize=address -g main.c -o main.o
	gcc -c -Wall -Werror -fsanitize=address -g part1.c -o part1.o
	gcc -c -Wall -Werror -fsanitize=address -g part2.c -o part2.o
	gcc -c -Wall -Werror -fsanitize=address -g part3.c -o part3.o
	gcc -c -Wall -Werror -fsanitize=address -g helper.c -o helper.o
	gcc -Wall -Werror -fsanitize=address -g mtll.o main.o part1.o part2.o part3.o helper.o -o mtll
