
VERSION = 0.1

CC      = gcc
CFLAGS  = -Wall -O2 -fomit-frame-pointer -pipe -DVERSION=\"$(VERSION)\"
CFLAGS += -Ilib/ -D_GNU_SOURCE
LDFLAGS = 

VPATH = src

.PHONY: all install disclean clean test dummy

PROGS =        basename   cal        cat        chgrp      chmod      \
	chown      chroot     cksum      cmp        colon      comm       \
	cut        date       dirname    domainname echo       expand     \
	false      fold       head       hostname   kill       link       \
	ln         logger     logname    ls         md5sum     mesg       \
	mkdir      mkfifo     mknod      more       newgrp     nl         \
	no         od         paste      prezip     printf     pwd        \
	rm         rmdir      rot13      seq        sleep      sort       \
	split      strings    sum        tabs       tac        tail       \
	tee        touch      tr         true       tty        uname      \
	unexpand   uniq       unlink     uptime     uudecode   uuencode   \
	wc         who        whoami     write      xargs      yes

LIB_FILES = lib/error.o lib/progname.o lib/terror.o lib/warning.o \
	    lib/getnum_ul.o lib/getnum_ull.o lib/getnum_l.o lib/getnum_ll.o \
	    lib/getnum_f.o lib/getnum_d.o lib/getnum_ld.o lib/getnum_hex.o \
	    lib/getnum_octal.o lib/yesno.o

objs = $(foreach cmd,$(PROGS), src/$(cmd).o)


all: bin/utils

bin/utils: src/utils.o $(objs) $(LIB_FILES)
	$(CC) $(LDFLAGS) -o $@ $^
	for cmd in $(PROGS); do \
		ln -sf utils bin/$$cmd ; \
	done

.c.o:
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm -f utils bin/* *.o `find . -name '*.o'`

