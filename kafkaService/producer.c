#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "producer.h"

char hostname[128];
char errstr[512];
rd_kafka_conf_t *conf;
rd_kafka_topic_conf_t *topic_conf;
rd_kafka_t *producer_handle;
rd_kafka_topic_t *topic;

void initialize_kafka_service(){

    conf = rd_kafka_conf_new();

    if (gethostname(hostname, sizeof(hostname))){
        fprintf(stderr, "%% Failed to lookup hostname\n");
        exit(1);
    }

    if (rd_kafka_conf_set(conf, "client.id" , hostname, errstr, sizeof(errstr)) != RD_KAFKA_CONF_OK){
        fprintf(stderr, "%% %s\n", errstr);
        exit(1);
    }

    if (rd_kafka_conf_set(conf, "bootstrap.servers", "host1:9092,host2:9092", errstr, sizeof(errstr)) != RD_KAFKA_CONF_OK){
        fprintf(stderr, "%% %s\n", errstr);
        exit(1);
    }

    /* Topic Configuration */
    topic_conf = rd_kafka_topic_conf_new();

    if (rd_kafka_topic_conf_set(topic_conf, "acks", "all", errstr, sizeof(errstr)) != RD_KAFKA_CONF_OK){
        fprintf(stderr, "%% %s\n", errstr);
        exit(1);
    }

    /* Create Kafka Producer Handle*/
    if (!(producer_handle = rd_kafka_new(RD_KAFKA_PRODUCER, conf, errstr, sizeof(errstr)))){
        fprintf(stderr, "%% Failed to create new producer: %s\n", errstr);
        exit(1);
    }
}

void produce(char* key, char* payload, int32_t partition){
    topic = rd_kafka_topic_new(producer_handle, TOPIC, topic_conf);
    if (rd_kafka_produce(topic, partition, RD_KAFKA_MSG_F_COPY, payload, strlen(payload), key, strlen(key), NULL) == -1){
        fprintf(stderr, "%% Failed to produce to topic %s: %s\n", TOPIC, rd_kafka_err2str(errno));
    }
}

void terminate_producer(){

    /* 1) Make sure all outstanding requests are transmitted and handled. */
    rd_kafka_flush(producer_handle, 60*1000); /* One minute timeout */

    /* 2) Destroy the topic and handle objects */
    rd_kafka_topic_destroy(topic);  /* Repeat for all topic objects held */
    rd_kafka_destroy(producer_handle);
}