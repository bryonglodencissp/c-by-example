#include <nmc.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
	char buf[64];
	nmc_sock_t sock = nmc_open_subscribe("226.0.0.1", 4096);

	while (1) {
		memset(buf, 0, sizeof(buf));
		nmc_recv(sock, buf, sizeof(buf));
		printf("%s", buf);
	}

	return 0;
}
