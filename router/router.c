// Author - Emeka Umeozo

#include "include/router.h"
#include "include/helper.h"

char my_name;
char* my_port;
router neighbours[MAX_KNOWN_ROUTES] = {0};
int routing_table[MAX_KNOWN_ROUTES][MAX_KNOWN_ROUTES] = {{0}};
int host_sock;
const int TIMEOUT = 10;
struct addrinfo* addr_info_list;
fd_set current_sockets, ready_sockets, closed_sockets;

int main(int argc, char* argv[]){

    int n_ports_start = 3;
    if (argc < (n_ports_start-1)){
        printf("Error: Specify router name and port\n");
        exit(EXIT_FAILURE);
    }

    my_name = *argv[1];
    my_port = argv[2];
    init_routers();

    /* Validate neighbour's ports and add neighbours to the routing table if their ports are valid */
    for (int i = n_ports_start; i < argc; i++) {
        if(!port_is_valid(argv[i])){

            printf("Error: Port %s given. \nPort numbers must range between %s and %s\n", argv[i], MIN_PORT_NO, MAX_PORT_NO);
            exit(EXIT_FAILURE);
        }

        neighbours[i-n_ports_start].is_post_linked = true;
        neighbours[i-n_ports_start].port = argv[i];
    }

    FD_ZERO(&current_sockets);
    init_routing_table();
    update_routing_table(my_name, my_name, 0);
    print_routing_table();
    run_router();
}

/*
 * Print router
 */
void print_router(router* r){
    if (r == NULL){
        printf("-\n");
    }else{
        printf("Node %c: port = %s, socket = %d, is_neighbour = %d\n", r->name, r->port, r->socket, r->is_post_linked);
    }
}

void print_routing_table(){
    for (int i = 0; i < MAX_KNOWN_ROUTES; ++i) {
        for (int j = 0; j < MAX_KNOWN_ROUTES; ++j) {
            if (routing_table[i][j] == 100){
                printf("-");
            }else{
                printf("%d ", routing_table[i][j]);
            }
        }
        printf("\n");
    }
    printf("\n\n");
}

/*
 * Set router attributes to default
 * --------------------------------
 * name = 0
 * socket = -1
 * port = ""
 * is_post_linked = false
 */
void set_router_to_default(router* r){

    r->name = 0;
    r->socket = -1;
    r->port = "";
    r->is_post_linked = false;
}

/*
 * Set every neighbour to default
 */
void init_routers(){
    for (int i = 0; i < MAX_KNOWN_ROUTES; ++i) {
        set_router_to_default(&neighbours[i]);
    }
}

/*
 * Set every position in router table to impossible number: 100
 */
void init_routing_table(){ // a route's distance can never be greater than INFINITY
    for (int i = 0; i < MAX_KNOWN_ROUTES; ++i) {
        for (int j = 0; j < MAX_KNOWN_ROUTES; ++j) {
            routing_table[i][j] = impossible_number;
        }
    }
}

/*
 * Get the number that is equivalent to the router's row in the routing table
 * Row index is dependent on the router's name
 */
int get_table_row_index(unsigned char c){
    int r_index = ((int)c) - 65; // A in ascii is 65
    return r_index;
}

unsigned char get_ascii_equivalent(int number){
    unsigned char c = number + 65;
    return c;
}

bool is_router_name_valid(unsigned char c){

    int r_index = get_table_row_index(c);
    if (r_index < 0 || r_index > 25){
        return false;
    }

    return true;
}

/*
 * Update the source router's cost to the destination router
 * Update this router's cost to the destination router using
 * the minimum cost to the destination from other routers
 */
void update_routing_table(unsigned char src, unsigned char dest, int cost){

    int src_index = get_table_row_index(src);
    int dest_index = get_table_row_index(dest);
    int my_index = get_table_row_index(my_name);

    routing_table[src_index][dest_index] = cost;

    if(src_index != my_index && dest_index != my_index){ // perform below if src and dest are foreign routers
        int cost_to_dest = INFINITY;
        for (int i = 0; i < MAX_KNOWN_ROUTES; i++) { // find min cost to destination router. traverse down rows
            if(i == my_index){
                // If this router has old value, we don't want it's recorded cost to dest to remain if cost increases
                continue;
            }
            int cost_to_neighbour = routing_table[my_index][i];
            if (cost_to_dest > (routing_table[i][dest_index] + cost_to_neighbour)){
                cost_to_dest = (routing_table[i][dest_index] + cost_to_neighbour);
            }
        }
        routing_table[my_index][dest_index] = cost_to_dest;
    }
}

void send_dist_to_known_routers(){

    route route_info;
    route_info.type = info;

    int my_index = get_table_row_index(my_name);
    int dist_to_dest;
    unsigned char next_hop = 0;
    unsigned char dest;

    for (int i = 0; i < MAX_KNOWN_ROUTES; ++i) { // for each known router to send info to
        if (neighbours[i].name != 0 && neighbours[i].socket != -1){
            if(DEBUG_MODE){
                printf("Sending to Neighbour %c\n", neighbours[i].name);
            }
            for (int dest_index = 0; dest_index < MAX_KNOWN_ROUTES; dest_index++) { // get cost to each known router
                dest = get_ascii_equivalent(dest_index);
                dist_to_dest = routing_table[my_index][dest_index];
                if (dist_to_dest != impossible_number){ // send info of routers whose distances are known only

                    // --- POISONED REVERSE:
                    // If x routes to y via z(next_hop), x will advertise to z that
                    // its shortest path length to y is infinity

                    next_hop = get_next_hop(dest);
                    if(DEBUG_MODE){
                        printf("Next hop to Neighbour %c = %c\n", dest, next_hop);
                    }
                    if(next_hop == neighbours[i].name && neighbours[i].name != dest){
                        if(DEBUG_MODE){
                            printf("setting cost to infinity\n");
                        }
                        // set infinity if next hop is neighbour being advertised to and if its not the destination route
                        route_info.cost = INFINITY;
                    }else{
                        route_info.cost = routing_table[my_index][dest_index];
                    }
                    // ---
                    route_info.dest = get_ascii_equivalent(dest_index);
                    route_info.src = my_name;
                    send_packet(neighbours[i].socket, &route_info, sizeof(route_info));
                }
            }
        }
    }
}

//void send_data_to_kafka_topic(){
//    int my_index = get_table_row_index(my_name);
//    for (int i = 0; i < MAX_KNOWN_ROUTES; i++) { // get cost to each router
//        routing_table[my_index][i];
//    }
//}

void run_router() {

    int addr_stat, conn_sock;
    struct addrinfo hints;
    struct timeval current_time, last_update_time = getTime();
    struct timeval wait_time = {2, 0};
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; /* unspecified - both ipv4 and ipv6 accepted */
    hints.ai_socktype = SOCK_STREAM; /* tcp */
    hints.ai_flags = AI_PASSIVE; /* need to bind to sockfd, fill in ip for me */

    addr_stat = getaddrinfo(NULL, my_port, &hints, &addr_info_list);
    host_sock = get_socket(addr_stat, addr_info_list, true);

    if (listen(host_sock, MAX_KNOWN_ROUTES) == -1) {
        perror("listen");
        exit(1);
    }

    //Initialize current set
    FD_SET(host_sock, &current_sockets);
    route route_info;

    for (;;) {
        current_time = getTime();
        if ((current_time.tv_sec - last_update_time.tv_sec) >= TIMEOUT) {
            connect_to_neighbours();
            send_dist_to_known_routers();
            print_next_hop_for_each_known_router();
            print_routing_table();
            last_update_time = getTime();
        }

        ready_sockets = current_sockets;
        closed_sockets = current_sockets;

        if (select(FD_SETSIZE, &ready_sockets, NULL, &closed_sockets, &wait_time) < 0) {
            perror("select error");
            exit(EXIT_FAILURE);
        }

        for (int socket = 0; socket < FD_SETSIZE; socket++) {
            if (FD_ISSET(socket, &ready_sockets)) {
                if(DEBUG_MODE){
                    printf("socket %d triggered!!\n", socket);
                }
                if (socket == host_sock) {
                    conn_sock = accept_new_connection(host_sock);
                    if(DEBUG_MODE){
                        printf("recvd conn request. setting socket to %d\n", socket);
                    }
                    FD_SET(conn_sock, &current_sockets);
                } else {
                    if (recv(socket, &route_info, sizeof(route_info), 0) > 0) {

                        if(DEBUG_MODE){
                            printf("recvd route info pack from socket %d\n", socket);
                            printf("type = %d \n", route_info.type);
                            printf("dest = %c \n", route_info.dest);
                            printf("src = %c \n", route_info.src);
                            printf("cost = %d \n", route_info.cost);
                        }
                        if (route_info.type == greet) {
                            route route_ack = {ack, my_name, my_name, 0};
                            update_routing_table(my_name, route_info.src, 1);
                            // Add router that connected to me to known routers
                            for (int i = 0; i < MAX_KNOWN_ROUTES; ++i) {
                                print_router(&neighbours[i]);
                                if (neighbours[i].socket == socket) {
                                    break;
                                }
                                if (neighbours[i].socket == -1 && !neighbours[i].is_post_linked) {
                                    neighbours[i].socket = socket;
                                    neighbours[i].name = route_info.src;
                                    break;
                                }
                            }
                            send_packet(socket, &route_ack, sizeof(route_ack));
                        } else if (route_info.type == ack) {
                            // find neighbour, set its name and set cost in routing table
                            for (int i = 0; i < MAX_KNOWN_ROUTES; ++i) {
                                if (neighbours[i].socket == socket) {
                                    neighbours[i].name = route_info.src;
                                    update_routing_table(my_name, route_info.src, 1);
                                    break;
                                }
                            }
                        } else if (route_info.type == info) {
                            update_routing_table(route_info.src, route_info.dest, route_info.cost);
                        }
                    } else {
                        for (int i = 0; i < MAX_KNOWN_ROUTES; ++i) {
                            if(neighbours[i].socket == socket){
                                printf("Router %c is offline\n", neighbours[i].name);
                                break;
                            }
                        }
                        FD_CLR(socket, &ready_sockets);
                        FD_CLR(socket, &current_sockets);
                        close(socket);
                    }
                    FD_CLR(socket, &ready_sockets);
                }
            }
        }
    }
}

void connect_to_neighbours(){
    struct addrinfo hints;
    int addr_stat;
    if(DEBUG_MODE){
        printf("Connecting to neighbours...\n");
    }
    for (int i = 0; i < MAX_KNOWN_ROUTES; ++i) {
        if (strcmp(neighbours[i].port, "") == 0 || !neighbours[i].is_post_linked){
            continue;
        }
        if (!is_connected(neighbours[i].socket)){
            memset(&hints, 0, sizeof hints);
            hints.ai_family = AF_UNSPEC; /* unspecified - both ipv4 and ipv6 accepted */
            hints.ai_socktype = SOCK_STREAM; /* tcp */
            hints.ai_flags = AI_PASSIVE; /* need to bind to sockfd, fill in ip for me */

            addr_stat = getaddrinfo(NULL, neighbours[i].port, &hints, &addr_info_list);
            // connect & set router's socket
            neighbours[i].socket = get_socket(addr_stat, addr_info_list, false);
            if (neighbours[i].socket != -1){
                if(DEBUG_MODE){
                    printf("Connected! socket = %d\n", neighbours[i].socket);
                }
                FD_SET(neighbours[i].socket, &current_sockets);
                greet_neighbour(neighbours[i].socket);
            }
        }else{
            if(DEBUG_MODE){
                printf("Already connected, socket = %d\n", neighbours[i].socket);
            }
        }
    }
}

/*
 * Check if socket is active by sending heartbeat packet
 * -----------------------------------------------------
 * If socket is active, return true
 * If socket is inactive, return false
 */
bool is_connected(int socket){

    long bytes;
    if (socket < 0){
        return false;
    }

    bytes = send_heartbeat(socket);
    if (bytes <= 0){
        return false;
    }

    return true;
}

/*
 * Send heartbeat packet
 * ---------------------
 * return bytes sent
 */
long send_heartbeat(int socket){
    long bytes;
    route r = {heartbeat, my_name, my_name, 0};
    bytes = send_packet(socket, &r, sizeof(r));

    return bytes;
}

/*
 * Print the name of the router(s) that supplied the minimum distance for each known router
 */
void print_next_hop_for_each_known_router(){
    int dist, my_index;
    my_index = get_table_row_index(my_name);
    for (int dest_index = 0; dest_index < MAX_KNOWN_ROUTES; dest_index++) {
        dist = routing_table[my_index][dest_index];
        if(dist == 100) {
            continue;
        }
        printf("Distance to %c = %d\n", get_ascii_equivalent(dest_index), dist);
        printf("next_hop = %c\n", get_next_hop(get_ascii_equivalent(dest_index)));
    }
}

/*
 * Get next hop to given router
 */
unsigned char get_next_hop(unsigned char router){

    unsigned char next_hop = 0;
    int cost = INFINITY;
    int my_index = get_table_row_index(my_name);
    int dest_index = get_table_row_index(router);
    int dist = routing_table[my_index][dest_index];

    if (router == my_name){
        return 0;
    }
    for (int src_index = 0; src_index < MAX_KNOWN_ROUTES; src_index++) {

        if(dist == 100) {continue;}

        // ignore when self is the source [self][dest] is assigned the next hop's cost
        if (my_index == src_index){continue;}

        if(cost > routing_table[src_index][dest_index]){
            cost = routing_table[src_index][dest_index];
            next_hop = get_ascii_equivalent(src_index);
        }
    }
    return next_hop;
}

long greet_neighbour(int socket){
    long bytes;
    route r = {greet, my_name, my_name, 0};
    bytes = send_packet(socket, &r, sizeof(r));
    return bytes;
}
