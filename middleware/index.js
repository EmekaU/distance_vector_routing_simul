const {Kafka} = require('kafkajs');

(async function consume(){

    const kafka = new Kafka({
        clientId: 'localhost',
        brokers: ['localhost:9092', 'localhost:9092'],
    })

    const consumer = kafka.consumer({groupId: 'middleware'})
    await consumer.connect()
    await consumer.subscribe({topic: 'routers', fromBeginning:true})
    await consumer.run({
        eachMessage: async ({topic, partition, message}) => {
            console.log({
                value: message.value.toString(),
            })
        },
    })
})();
