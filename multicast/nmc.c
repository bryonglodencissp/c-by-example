//#define _XOPEN_SOURCE 700
#include <nmc.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

struct nmc_sock {
	int fd;
	char *ip;
	uint16_t port;
	enum nmc_sock_types type;
	struct ip_mreq imreq;
	struct sockaddr_in saddr;
	struct in_addr iaddr;
};

static struct nmc_sock *nmc_create(void)
{
	struct nmc_sock *sock;

	sock = calloc(sizeof(*sock), 1);
	if (!sock)
		return NULL;

	sock->fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);
	if (sock->fd < 0) {
		free(sock);
		return NULL;
	}

	return sock;
}

static struct nmc_sock *nmc_open_pub(const char *ip, uint16_t port)
{
	int ret;
	struct nmc_sock *sock;
	unsigned char ttl = 3;

	sock = nmc_create();
	if (!sock)
		return sock;

	sock->saddr.sin_family = PF_INET;
	sock->saddr.sin_port = htons(0); // INPORT_ANY
	sock->saddr.sin_addr.s_addr = htonl(INADDR_ANY);

	ret = bind(sock->fd, (struct sockaddr *)&sock->saddr, sizeof(struct sockaddr_in));
	if (ret < 0) {
		nmc_close(&sock);
		return NULL;
	}

	// Set the outgoing interface to DEFAULT
	setsockopt(sock->fd, IPPROTO_IP, IP_MULTICAST_IF, &sock->iaddr, sizeof(struct in_addr));

	// Set multicast packet TTL to 3; default TTL is 1
	setsockopt(sock->fd, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(unsigned char));

	// set destination multicast address
	sock->saddr.sin_family = PF_INET;
	sock->saddr.sin_addr.s_addr = inet_addr(ip);
	sock->saddr.sin_port = htons(port);

	return sock;
}

static struct nmc_sock *nmc_open_sub(const char *ip, uint16_t port)
{
	int ret;
	struct nmc_sock *sock;

	sock = nmc_create();
	if (!sock)
		return sock;

	sock->saddr.sin_family = PF_INET;
	sock->saddr.sin_port = htons(port); // INPORT_ANY
	sock->saddr.sin_addr.s_addr = htonl(INADDR_ANY);

	ret = bind(sock->fd, (struct sockaddr *)&sock->saddr, sizeof(struct sockaddr_in));
	if (ret < 0) {
		nmc_close(&sock);
		return NULL;
	}

	sock->imreq.imr_multiaddr.s_addr = inet_addr(ip);
	sock->imreq.imr_interface.s_addr = INADDR_ANY;

	// JOIN multicast group on default interface
	setsockopt(sock->fd, IPPROTO_IP, IP_ADD_MEMBERSHIP,
		(const void *)&sock->imreq, sizeof(struct ip_mreq));

	return sock;
}

nmc_sock_t nmc_open(enum nmc_sock_types type, const char *ip, uint16_t port)
{
	nmc_sock_t s = NULL;

	switch (type) {
	case NMC_SOCK_TYPE_PUB:
		s = nmc_open_pub(ip, port);
		break;
	case NMC_SOCK_TYPE_SUB:
		s = nmc_open_sub(ip, port);
		break;
	}

	return s;
}

size_t nmc_send(struct nmc_sock *sock, void *data, size_t len)
{
	socklen_t socklen = sizeof(struct sockaddr_in);
	return sendto(sock->fd, data, len, 0, (struct sockaddr *)&sock->saddr, socklen);
}

size_t nmc_recv(struct nmc_sock *sock, void *data, size_t len)
{
	socklen_t socklen = sizeof(struct sockaddr_in);
	return recvfrom(sock->fd, data, len, 0, (struct sockaddr *)&sock->saddr, &socklen);
}

void nmc_close(struct nmc_sock **sock)
{
	shutdown((*sock)->fd, 2);
	close((*sock)->fd);
	free(*sock);
	*sock = NULL;
}
