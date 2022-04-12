// Author - Emeka Umeozo

#ifndef DISTANCE_VECTOR_ROUTING_SIMUL_ROUTER_H
#define DISTANCE_VECTOR_ROUTING_SIMUL_ROUTER_H
#include "imports.h"
#define impossible_number 100
typedef enum route_type{
    greet = 0, ack = 1, info = 2, heartbeat = 3
}route_type;

typedef struct route{
    route_type type;
    unsigned char dest;
    unsigned char src;
    int cost;
}route;

typedef struct router{
    unsigned char name;
    int socket;
    char* port;
    bool is_post_linked; //flags a router that running router connects to.
}router;

void set_router_to_default(router* r);
void init_routers();
void print_router(router* r);
void print_routing_table();
int get_table_row_index(unsigned char c);
bool is_router_name_valid(unsigned char c);
void send_data_to_kafka_topic();
void init_routing_table();
void update_routing_table(unsigned char src, unsigned char dest, int cost);
void run_router();
void connect_to_neighbours();
bool is_connected(int socket);
long send_heartbeat(int socket);
void print_next_hop_for_each_known_router();
unsigned char get_next_hop(unsigned char router);
long greet_neighbour(int socket);
#endif //DISTANCE_VECTOR_ROUTING_SIMUL_ROUTER_H
