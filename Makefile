INCLUDE=-I./ -I./include
LIBS= ./lib/glibc/libimp.a ./lib/glibc/libalog.a -lpthread -lm -lrt -ldl
all:JY-529V
JY-529V:
	 mips-linux-gnu-gcc -o JY-529V -O2 -w -march=mips32r2 Main.cpp CommFun.cpp TimerDeal.cpp UartDevice.cpp UsartDeal.cpp EepromDeal.cpp iniparser.c dictionary.c PingDeal.cpp NtpDeal.cpp ConnServiceDeal.cpp NetDataDeal.cpp WiredNetDeal.cpp EventDeal.cpp KeyDeal.cpp PlayPcm.cpp sample-common.c $(INCLUDE) $(LIBS)
clean:
	rm -rfv JY-529V