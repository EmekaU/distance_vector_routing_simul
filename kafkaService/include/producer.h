// Author - Emeka Umeozo

#ifndef DISTANCE_VECTOR_ROUTING_SIMUL_KAFKAPROP_H
#define DISTANCE_VECTOR_ROUTING_SIMUL_KAFKAPROP_H
#include <librdkafka/rdkafka.h>
#define TOPIC "routes"

//int create_producer(rd_kafka_t **rk, rd_kafka_conf_t *conf, char *errstr);
//void *produce(rd_kafka_t **rk, char* key, char* payload);
void produce(char* key, void* payload);
void initialize_kafka_service();
#endif //DISTANCE_VECTOR_ROUTING_SIMUL_KAFKAPROP_H
