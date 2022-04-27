// Author - Emeka Umeozo

#ifndef DISTANCE_VECTOR_ROUTING_SIMUL_KAFKAPROP_H
#define DISTANCE_VECTOR_ROUTING_SIMUL_KAFKAPROP_H
#include <librdkafka/rdkafka.h>
#define TOPIC "routes"

void initialize_kafka_service();
void produce(char* key, char* payload, int32_t partition);
void terminate_producer();
#endif //DISTANCE_VECTOR_ROUTING_SIMUL_KAFKAPROP_H
