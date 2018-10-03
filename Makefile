# Name: Kwan Ho Herman Tong, Jessica Hardey, Josefa O'Sorio
# NetId: ktong1, jhardey, josorio2

CXX=		g++
CXFLAGS=	-g -std=c++11
TARGETS=	server/bin/myftpd client/bin/myftp

all: $(TARGETS)

server/bin/myftpd: server/bin/myftpd.o server/bin/server_op.o bin/network_utils.o
	$(CXX) $(CXFLAGS) -o $@ $^

client/bin/myftp: client/bin/myftp.o client/bin/client_op.o bin/network_utils.o
	$(CXX) $(CXFLAGS) -o $@ $^

server/bin/myftpd.o: server/myftpd.cpp server/myftpd.h
	mkdir -p server/bin
	$(CXX) $(CXFLAGS) -o $@ -c $<

server/bin/server_op.o: server/server_op.cpp server/myftpd.h
	$(CXX) $(CXFLAGS) -o $@ -c $<

client/bin/myftp.o: client/myftp.cpp client/myftp.h
	mkdir -p client/bin
	$(CXX) $(CXFLAGS) -o $@ -c $<

client/bin/client_op.o: client/client_op.cpp client/myftp.h
	$(CXX) $(CXFLAGS) -o $@ -c $<

bin/network_utils.o: network_utils/network_utils.cpp network_utils/network_utils.h
	mkdir -p bin
	$(CXX) $(CXFLAGS) -o $@ -c $<

clean:
	rm $(TARGETS) bin/*.o client/bin/*.o server/bin/*.o
