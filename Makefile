src	= $(wildcard *.c)
app	= $(patsubst %.c,%,$(src))

all:$(app)

$(app):%:%.c
	gcc $^ -o $@ -lpthread
clean:
	rm -rf $(app)
