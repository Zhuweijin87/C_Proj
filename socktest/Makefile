target=server client selserver udpserver udpclient netaddr sockpacket
all:$(target)
server:socket_server.o
	cc -o $@ $<
client:socket_client.o
	cc -o $@ $<
selserver:select_server.o
	cc -o $@ $<
udpserver:udp_server.o
	cc -o $@ $<
udpclient:udp_client.o
	cc -o $@ $<
netaddr:netaddr.o
	cc -o $@ $<
sockpacket:socket_packet.o
	cc -o $@ $<
clean:
	rm -f *.o $(target)
