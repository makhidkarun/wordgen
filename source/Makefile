CFLAGS = -DSYSV
#CFLAGS = -DBSD

word: word.c
	cc -o word word.c fgetline.c word-random.c $(CFLAGS) -lm
