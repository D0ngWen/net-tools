all: build udpechoclient udpechoserver

build:
	mkdir out

udpechoclient: cli.c
	gcc cli.c -g -o ./out/cli

udpechoserver: srv.c
	gcc srv.c -g -o ./out/srv


clean:
	$(RM) -r out