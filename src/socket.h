#ifndef	_SOCKET_H
#define	_SOCKET_H 1

#include <sys/select.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define SOCKET_NAME "smu.socket"


/**
 * @brief 파일 설명자를 생성합니다.
 *
 * 유닉스 내부 네트워크를 사용하며, 생성 과정에서
 * 오류가 있다면 에러코드 1과 함께 프로그램을 종료합니다.
 *
 * @param sock_fd
*/
extern void socket_init_fd(int *sock_fd);

/**
 *
*/
extern void socket_init_addr(struct sockaddr_un *sock_addr);
extern void socket_connect(const int sock_fd, const struct sockaddr_un *sock_addr);
extern void socket_bind(const int sock_fd, const struct sockaddr_un *sock_addr);
extern void socket_listen(const int sock_fd, const int n_connections);
extern int socket_accept(const int sock_fd, const struct sockaddr_un *sock_addr);

#endif
