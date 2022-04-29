# Distance Vector Routing Simulation
## About the Program

This program is designed to simulate the distance vector algorithm using router processes.
Each process maintains a routing table and sends to its neighbours, its best known distance to each destination router.
Periodically, each router prints its router table, sends its distances its neighbour routers and to the kafka topic.

Check out the `documentation.txt` file for more details.

## RoadMap
- [x] Routing Algorithm
- [x] Kafka Integration(production)
- [ ] Vue Interface

## Dependencies

- librdkafka
- docker & docker-compose

## How To Run the Program
### Command Line:
- Enter the `distance_vector_routing_simul/` directory.
- Run `./configure.sh` to build cmake dependencies.
- Run `./build.sh` to link libraries and build executable.
- Enter `build/router/dvRouter <router name> <router port> <neighbour ports>` to run the router.

### Vue Interface:
- In progress