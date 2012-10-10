all: utils

commands = cat head od tac tail yes

objs = $(foreach cmd,$(commands), $(cmd).o)

utils: utils.o $(objs)
	gcc -o utils utils.o $(objs)
	for command in $(commands); do \
		ln -sf utils $$command ; \
	done

.c.o:
	gcc -c -Wall -O2 -fomit-frame-pointer $<

clean:
	rm -f *.o utils $(commands)
