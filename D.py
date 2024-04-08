import heapq

def dijkstra(graph, start):
    # Dictionary to store shortest distances
    distances = {node: float('inf') for node in graph}
    distances[start] = 0
    
    # Priority queue to store nodes to visit next
    pq = [(0, start)]
    
    while pq:
        # Pop the node with the smallest distance
        current_distance, current_node = heapq.heappop(pq)
        
        # Visit each neighbor of the current node
        for neighbor, weight in graph[current_node].items():
            distance = current_distance + weight
            
            # Update the distance if shorter path is found
            if distance < distances[neighbor]:
                distances[neighbor] = distance
                heapq.heappush(pq, (distance, neighbor))
    
    return distances

# Example graph
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

# Run Dijkstra's algorithm
shortest_distances = dijkstra(graph, start_node)

print("Shortest distances from node", start_node)
for node, distance in shortest_distances.items():
    print("Node:", node, "Distance:", distance)
