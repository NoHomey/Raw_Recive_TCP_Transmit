sniffer: rm_sniffer
	gcc Sniffer.c -Wall -o sniffer && ./sniffer

server: rm_server
	gcc Server.c -Wall -o server && ./server a

listener: rm_server
	gcc Server.c -Wall -o server && ./server n

sender: rm_sender
	gcc Sender.c -Wall -o sender && ./sender

rm_server:
	rm -f server

rm_sniffer:
	rm -f sniffer

rm_sender:
	rm -f sender

clean:
	rm -f server sniffer sender *~
