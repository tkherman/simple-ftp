# Name: Kwan Ho Herman Tong, Jessica Hardey, Josefa O'Sorio
# NetId: ktong1, jhardey, josorio2

CXX=		g++
CXFLAGS=	-g -std=c++11
TARGETS=	myftpd myftp

all: $(TARGETS)

myftpd: myftpd.o server_op.o network_utils.o
	$(CXX) $(CXFLAGS) -o $@ $^

myftp: myftp.o client_op.o network_utils.o
	$(CXX) $(CXFLAGS) -o $@ $^

myftpd.o: myftpd.cpp myftpd.h
	$(CXX) $(CXFLAGS) -o $@ -c $<

server_op.o: server_op.cpp myftp.h
	$(CXX) $(CXFLAGS) -o $@ -c $<

myftp.o: myftp.cpp myftp.h
	$(CXX) $(CXFLAGS) -o $@ -c $<

client_op.o: client_op.cpp myftp.h
	$(CXX) $(CXFLAGS) -o $@ -c $<

network_utils.o: network_utils.cpp network_utils.h
	$(CXX) $(CXFLAGS) -o $@ -c $<

clean:
	rm $(TARGETS) *.o
