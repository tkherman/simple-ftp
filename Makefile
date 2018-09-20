# Name: Kwan Ho Herman Tong, Jessica Hardey, Josefa O'Sorio
# NetId: ktong1, jhardey, josorio2

CXX=		g++
CXFLAGS=	-g -std=c++11
TARGETS=	myftpd

all: $(TARGETS)

myftpd: myftpd.cpp myftpd.h
	$(CXX) $(CXFLAGS) -o $@ $<
