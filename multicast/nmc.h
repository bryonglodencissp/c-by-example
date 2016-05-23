/**
 * Simple network multicast i/o
 */
#ifndef NMC_H_
#define NMC_H_

#include <stdlib.h>
#include <stdint.h>

typedef struct nmc_sock *nmc_sock_t;

nmc_sock_t nmc_open_publish(const char *ip, uint16_t port);
nmc_sock_t nmc_open_subscribe(const char *ip, uint16_t port);
void nmc_close(nmc_sock_t *sock);
size_t nmc_send(struct nmc_sock *sock, void *data, size_t len);
size_t nmc_recv(struct nmc_sock *sock, void *data, size_t len);

#endif /* NMC_H_ */
