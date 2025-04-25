# Makefile for project1 web server
CC = gcc
CFLAGS = -Wall -g

TARGET = myserver

# 기본 규칙: make 실행 시 수행
all: $(TARGET)

# 실행 파일 생성 규칙
$(TARGET): myserver.c
	$(CC) $(CFLAGS) -o $(TARGET) myserver.c

# make clean 시 객체 파일/실행 파일 삭제
clean:
	rm -f $(TARGET)
