sniffer: rm_sniffer listner
	gcc Sniffer.c -o sniffer && ./sniffer

server: rm_server
	gcc Server.c -o server && ./server a

listner: rm_server
	gcc Server.c -o server && ./server n

sender: rm_sender
	gcc Sender.c -o sender && ./sender

rm_server:
	rm -f server

rm_sniffer:
	rm -f sniffer

rm_sender:
	rm -f sender
