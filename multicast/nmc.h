/**
 * Simple network multicast i/o
 */
#ifndef NMC_H_
#define NMC_H_

#include <stdlib.h>
#include <stdint.h>

typedef struct nmc_sock *nmc_sock_t;

enum nmc_sock_types {
	NMC_SOCK_TYPE_PUB,
	NMC_SOCK_TYPE_SUB
};

nmc_sock_t nmc_open(enum nmc_sock_types type, const char *ip, uint16_t port);
void nmc_close(nmc_sock_t *sock);
size_t nmc_send(nmc_sock_t sock, void *data, size_t len);
size_t nmc_recv(nmc_sock_t sock, void *data, size_t len);

#endif /* NMC_H_ */
