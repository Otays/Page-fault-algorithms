CFLAGS = -w -I. 

pagerep: pagerep.o colorlogs.o
	gcc $(CFLAGS) -o pagerep pagerep.o colorlogs.o

colorlogs.o: colorlogs.c
	gcc $(CFLAGS) -c colorlogs.c
	
pagerep.o: pagerep.c
	gcc $(CFLAGS) -c pagerep.c 
	
