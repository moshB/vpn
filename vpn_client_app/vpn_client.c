#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <linux/if.h>
#include <linux/if_tun.h>

#define MTU 1500
#define SERVER_PORT 5000
/*5.29.13.173  ip.addr == 188.64.207.196 */
#define SERVER_IP "10.1.0.57" 
#define KEY "mysecretkey"


int tun_alloc(char *dev_name) 
{
    struct ifreq ifr;
    int fd, err;

    if ((fd = open("/dev/net/tun", O_RDWR)) < 0) 
    {
        perror("Opening /dev/net/tun");
        return fd;
    }

    memset(&ifr, 0, sizeof(ifr));
    ifr.ifr_flags = IFF_TUN;

    if (*dev_name) 
    {
        strncpy(ifr.ifr_name, dev_name, IFNAMSIZ);
    }

    if ((err = ioctl(fd, TUNSETIFF, (void *)&ifr)) < 0) 
    {
        perror("ioctl(TUNSETIFF)");
        close(fd);
        return err;
    }

    strcpy(dev_name, ifr.ifr_name);
    return fd;
}

void encrypt_data(unsigned char *data, int len, const char *key) 
{
    for (int i = 0; i < len; ++i) {
        data[i] = data[i] ^ key[i % strlen(key)];
    }
}

void decrypt_data(unsigned char *data, int len, const char *key) 
{
    for (int i = 0; i < len; ++i) {
        data[i] = data[i] ^ key[i % strlen(key)];
    }
}

int main(void) {
    char tun_name[IFNAMSIZ] = "tun1"; 
    int tun_fd, udp_fd, max_fd;
    unsigned char buffer[MTU];
    ssize_t n_read;
    fd_set read_fds;
    struct sockaddr_in server_addr;

    printf("Creating TUN device '%s'...\n", tun_name);
    tun_fd = tun_alloc(tun_name);
    if (tun_fd < 0) {
        fprintf(stderr, "Failed to create TUN device.\n");
        exit(1);
    }
    printf("Successfully created TUN device '%s'.\n", tun_name);

    if ((udp_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }
    printf("Successfully created UDP socket.\n");

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("inet_pton");
        close(udp_fd);
        exit(1);
    }
    max_fd = (tun_fd > udp_fd) ? tun_fd : udp_fd;

    printf("Entering main loop...\n");
    while (1) {
        FD_ZERO(&read_fds);
        FD_SET(tun_fd, &read_fds);
        FD_SET(udp_fd, &read_fds);

        if (select(max_fd + 1, &read_fds, NULL, NULL, NULL) < 0) {
            perror("select");
            break;
        }

        /* if from tun */
        if (FD_ISSET(tun_fd, &read_fds)) {
            n_read = read(tun_fd, buffer, MTU);
            if (n_read < 0) {
                perror("read from tun");
                continue;
            }
            printf("Read %ld bytes from tun device.\n", n_read);
            encrypt_data(buffer, n_read, KEY);

            ssize_t n_sent = sendto(udp_fd, buffer, n_read, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
            if (n_sent < 0) {
                perror("sendto");
            }
        }

        /* if from server */
        if (FD_ISSET(udp_fd, &read_fds)) {
            n_read = recvfrom(udp_fd, buffer, MTU, 0, NULL, NULL);
            if (n_read < 0) {
                perror("recvfrom");
                continue;
            }
            printf("Received %ld bytes from server.\n", n_read);
            decrypt_data(buffer, n_read, KEY);
            ssize_t n_written = write(tun_fd, buffer, n_read);
            if (n_written < 0) {
                perror("write to tun");
            }
        }
    }

    close(tun_fd);
    close(udp_fd);
    return 0;
}