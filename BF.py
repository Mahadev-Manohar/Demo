def bellman_ford(graph, start):
    # Initialize distances to all nodes as infinity, except the start node
    distances = {node: float('inf') for node in graph}
    distances[start] = 0
    
    # Relax edges repeatedly
    for _ in range(len(graph) - 1):
        for node in graph:
            for neighbor, weight in graph[node].items():
                if distances[node] + weight < distances[neighbor]:
                    distances[neighbor] = distances[node] + weight
    
    # Check for negative weight cycles
    for node in graph:
        for neighbor, weight in graph[node].items():
            if distances[node] + weight < distances[neighbor]:
                raise ValueError("Graph contains a negative weight cycle")
    
    return distances

# Example graph
#raph = {
#    'A': {'B': -1, 'C': 4},
#    'B': {'C': 3, 'D': 2, 'E': 2},
#    'C': {},
#    'D': {'B': 1, 'C': 5},
#    'E': {'D': -3}
#}

graph = {
    'A': {'B': 5, 'C': 3},
    'B': {'A': 5, 'C': 2, 'D': 1},
    'C': {'A': 3, 'B': 2, 'D': 4, 'E': 2},
    'D': {'B': 1, 'C': 4, 'E': 3, 'F': 6},
    'E': {'C': 2, 'D': 3, 'F': 7},
    'F': {'D': 6, 'E': 7}
}

# Starting node
start_node = 'A'

# Run Bellman-Ford algorithm
shortest_distances = bellman_ford(graph, start_node)

print("Shortest distances from node", start_node)
for node, distance in shortest_distances.items():
    print("Node:", node, "Distance:", distance)
