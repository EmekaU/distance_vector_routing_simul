# Distance Vector Routing Simulation
## About the Program

This program is designed to show how the distance routing protocol works with the [poisoned reverse algorithm](https://en.wikipedia.org/wiki/Split_horizon_route_advertisement).
The distance vector routing protocol uses the bellman-ford algorithm to share its distance to other routers.
Each router maintains a routing table that contains the distance between itself and all possible destination nodes.
Every 10 seconds, or when the routing table changes, each router prints its router table, sends its distances its neighbour routers and to the kafka topic.

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
  (Max number of neighbours = 25)
### Vue Interface:
- In progress
