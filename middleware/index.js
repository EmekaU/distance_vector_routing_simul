const {Kafka} = require('kafkajs')

const run = async () => {
    const kafka = new Kafka({
        clientId: 'localhost',
        brokers: ['localhost:9092'],
    })
    const consumer = kafka.consumer({groupId: 'middleware'})
    await consumer.connect()
    await consumer.subscribe({topic: 'routers', fromBeginning: true})
    await consumer.run({
        eachMessage: async ({topic, partition, message}) => {
            console.log({
                partition,
                offset: message.offset,
                value: message.value.toString(),
            })
        }
    })
}

run().catch(console.error)