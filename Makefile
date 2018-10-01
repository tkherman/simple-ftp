# Name: Kwan Ho Herman Tong, Jessica Hardey, Josefa O'Sorio
# NetId: ktong1, jhardey, josorio2

CXX=		g++
CXFLAGS=	-g -std=c++11
TARGETS=	bin/myftpd bin/myftp

all: $(TARGETS)

bin/myftpd: bin/myftpd.o bin/server_op.o bin/network_utils.o
	$(CXX) $(CXFLAGS) -o $@ $^

bin/myftp: bin/myftp.o bin/client_op.o bin/network_utils.o
	$(CXX) $(CXFLAGS) -o $@ $^

bin/myftpd.o: server/myftpd.cpp server/myftpd.h
	$(CXX) $(CXFLAGS) -o $@ -c $<

bin/server_op.o: server/server_op.cpp server/myftpd.h
	$(CXX) $(CXFLAGS) -o $@ -c $<

bin/myftp.o: client/myftp.cpp client/myftp.h
	$(CXX) $(CXFLAGS) -o $@ -c $<

bin/client_op.o: client/client_op.cpp client/myftp.h
	$(CXX) $(CXFLAGS) -o $@ -c $<

bin/network_utils.o: network_utils/network_utils.cpp network_utils/network_utils.h
	$(CXX) $(CXFLAGS) -o $@ -c $<

clean:
	rm $(TARGETS) bin/*.o
