#include <nmc.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
	unsigned int cntr = 0;
	uint8_t buf[64];
	nmc_sock_t sock = nmc_open(NMC_SOCK_TYPE_PUB, "226.0.0.1", 4096);

	while (1) {
		int len = snprintf((char *)&buf, sizeof(buf), "client %u\n", cntr);
		nmc_send(sock, buf, len);
		sleep(1);
		cntr++;
	}

	nmc_close(&sock);
}
