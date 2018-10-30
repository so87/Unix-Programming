dummy: ntcp_client ntcp_server

ntcp_server: ntcp_server.cpp
	g++ -o ntcp_server ntcp_server.cpp

ntcp_client: ntcp_client.cpp
	g++ -o ntcp_client ntcp_client.cpp

clean: 
	rm -f ntcp_client ntcp_server

realclean: clean
	rm -f *~
