#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>

int TEST_tcp_wait(uint16_t port)
{
	struct conn {
		int fd;
		struct sockaddr_in addr;
		socklen_t addrlen;
	};

	struct conn server;
	struct conn client;

	server.fd = socket(AF_INET, SOCK_STREAM, 0);
	if(server.fd < 0)
		printf("Failed creating socket\n");

	memset(&server.addr, 0, sizeof(server.addr));
	server.addr.sin_family = AF_INET;
	server.addr.sin_addr.s_addr = INADDR_ANY;
	server.addr.sin_port = htons(port);

	if (bind(server.fd, (struct sockaddr *)&server.addr, sizeof(server.addr)) < 0)
		printf("Failed to bind\n");
	listen(server.fd, 0);

	client.addrlen = sizeof(client.addr);

	client.fd = accept(server.fd, (struct sockaddr *)&client.addr, &client.addrlen);
	if (client.fd == -1)
		printf("Failed accepting connection\n");
	else
		printf("Got connection from %s:%d\n",
			inet_ntoa(client.addr.sin_addr),
			(int) ntohs(client.addr.sin_port));

	close(client.fd);
	close(server.fd);

	return 0;
}

int TEST_tcp_connect(const char *host, uint16_t port)
{
	/* Variable and structure definitions. */
	int fd, ret;
	struct sockaddr_in addr;
	struct hostent *hostp;

	if((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		exit(-EBADF);

	memset(&addr, 0, sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_port   = htons(port);
 
	if((addr.sin_addr.s_addr = inet_addr(host)) == (unsigned long)INADDR_NONE)
	{
		hostp = gethostbyname(host);
		if(hostp == NULL) {
			ret = -EFAULT;
			goto err;
		}
		memcpy(&addr.sin_addr, hostp->h_addr, sizeof(addr.sin_addr));
	}

	ret = connect(fd, (struct sockaddr *)&addr, sizeof(addr));
	if (ret < 0) {
		printf("Could not connect to %s:%d\n",
			inet_ntoa(addr.sin_addr),
			(int) ntohs(addr.sin_port));

		ret = -ENXIO;
		goto err;
	}

	printf("Connected to %s:%d\n",
		inet_ntoa(addr.sin_addr),
		(int) ntohs(addr.sin_port));

err:
	close(fd);
	return ret;
}

void usage(const char *prg)
{
	fprintf(stderr, "%s:\n\t<server> <port>\n\t<client> <host> <port>\n", prg);
}

int main(int argc, char **argv)
{
	int ret = 0;
	uint16_t port;

	if (argc < 2) {
		usage(argv[0]);
		exit(EXIT_FAILURE);
	}

	switch(argv[1][0]) {
	case 's':
		if (argc < 2) {
			usage(argv[0]);
			ret = -EINVAL;
			break;
		}
		port = strtoul(argv[2], NULL, 0);
		ret = TEST_tcp_wait(port);
	break;
	case 'c':
		if (argc < 3) {
			usage(argv[0]);
			ret = -EINVAL;
			break;
		}
		port = strtoul(argv[3], NULL, 0);
		ret = TEST_tcp_connect(argv[2], port);
	break;
	default:
		usage(argv[0]);
		ret = EXIT_FAILURE;
	};

	return ret;
}
