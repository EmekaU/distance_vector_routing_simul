// Author - Emeka Umeozo
#include "helper.h"

int accept_new_connection(int server_socket){
    int conn_socket;
    struct sockaddr_storage incoming_addr;
    uint addr_length = sizeof incoming_addr;
    conn_socket = accept(server_socket, (struct sockaddr*) &incoming_addr, &addr_length);
    verify(conn_socket, "router: accept");

    return conn_socket;
}

void verify(int val, char* msg){
    if (val == -1){
        perror(msg);
        exit(EXIT_FAILURE);
    }
}

bool port_is_valid(char* port){

    if (atoi(port) < atoi(MIN_PORT_NO) || atoi(port) > atoi(MAX_PORT_NO)){
        return false;
    }
    return true;
}

long send_packet(int socket, void* packet, int packet_size){
    long num_bytes;
    if((num_bytes = send(socket, packet, packet_size, 0)) == -1){
        close(socket);
        fprintf(stderr, "tcp: can't send\n");
    }

    return num_bytes;
}

struct timeval getTime(){

    struct timeval time;
    gettimeofday(&time, 0);

    return time;
}

int get_socket(int addrinfo_status, struct addrinfo* addr_info_list, bool is_host){

    int sockfd;
    int reuse_socket = 1;
    struct addrinfo* ref;

    if(addrinfo_status != 0){

        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(addrinfo_status));
        exit(EXIT_FAILURE);
    }

    /* Loop through linkedlist, find first bindable address */
    for(ref = addr_info_list; ref != NULL; ref = ref->ai_next){

        /* get sockfd via socket call with domain, type, and protocol*/
        sockfd = socket(ref->ai_family, ref->ai_socktype, ref->ai_protocol);
        printf("sockfd = %d \n", sockfd);
        if(sockfd == -1){
            perror("invalid socket\n");
            continue;
        }

        if (is_host == true){
            if(bind(sockfd, ref->ai_addr, ref->ai_addrlen) == -1){
                /**
                 * Bind to associate port with the socket file descriptor, addr ref and length of ref in bytes
                 * kernel will associate sockfd with the port in the addrinfo struct
                 */
                close(sockfd); /* close file descriptor */
                perror("server: bind\n");
                continue;
            }

            if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse_socket, sizeof reuse_socket) == -1){
                perror("server: setsockopt\n");
                continue;
            }
        }
        else{
            if(connect(sockfd, ref->ai_addr, ref->ai_addrlen) == -1){
                close(sockfd);
                perror("client: connect");
                continue;
            }
        }
        break;
    }

    if(ref == NULL){ // only exit program if host fails to bind to socket
        sockfd = -1;
        if (is_host == true){
            fprintf(stderr, "server: failed to bind/connect\n");
            exit(1);
        }
    }

    freeaddrinfo(addr_info_list);
    return sockfd;
}
