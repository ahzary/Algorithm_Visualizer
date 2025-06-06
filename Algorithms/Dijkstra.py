import heapq
from typing import List, Tuple, Dict, Optional

class Algorithm:
    def __init__(self, map: List[List[int]], startPoint: List[int], endPoint: List[int]):
        self.map = map
        self.start = tuple(startPoint)
        self.end = tuple(endPoint)
        self.open_set = []
        self.came_from: Dict[Tuple[int, int], Optional[Tuple[int, int]]] = {}
        self.dist: Dict[Tuple[int, int], float] = {self.start: 0}
        self.current = self.start
        self.running = True
        self.name = __name__

        # Initialize open set with start node
        heapq.heappush(self.open_set, (0, self.start))
    
    def get_neighbors(self, pos: Tuple[int, int]) -> List[Tuple[int, int]]:
        """Get walkable neighbors (4-directional)"""
        directions = [(0,1), (1,0), (0,-1), (-1,0)]
        neighbors = []
        x, y = pos
        
        for dx, dy in directions:
            nx, ny = x + dx, y + dy
            if (0 <= nx < len(self.map)) and (0 <= ny < len(self.map[0])):
                if self.map[nx][ny] in [1, 6]:  # 1 (walkable), 6 (end square)
                    neighbors.append((nx, ny))
        return neighbors
    
    def step(self) -> List[List[int]]:
        """Execute one step of Dijkstra's algorithm processing multiple nodes at once"""
        if not self.open_set or not self.running:
            self.running = False
            return []

        # Get all nodes with the current minimum distance
        current_dist, current_nodes = self.open_set[0][0], []
        while self.open_set and self.open_set[0][0] == current_dist:
            _, node = heapq.heappop(self.open_set)
            current_nodes.append(node)
        
        changes = []
        for current in current_nodes:
            self.current = current
            
            # Check if we've reached the goal
            if current == self.end:
                self.running = False
                return self.reconstruct_path()
            
            # Explore neighbors
            for neighbor in self.get_neighbors(current):
                tentative_dist = self.dist[current] + 1  # 1 is the cost between nodes
                
                if neighbor not in self.dist or tentative_dist < self.dist[neighbor]:
                    self.came_from[neighbor] = current
                    self.dist[neighbor] = tentative_dist
                    heapq.heappush(self.open_set, (tentative_dist, neighbor))
                    
                    # Mark as explored (type 7, yellow)
                    if self.map[neighbor[0]][neighbor[1]] != 6:
                        changes.append([neighbor[0], neighbor[1], 7])
            
            # Mark current position (type 3, red)
            changes.append([current[0], current[1], 3])
        return changes
    
    def reconstruct_path(self) -> List[List[int]]:
        """Reconstruct the final path"""
        path = []
        current = self.end
        while current in self.came_from:
            path.append([current[0], current[1], 2])  # 2 represents final path (blue)
            current = self.came_from[current]
        path.append([self.start[0], self.start[1], 2])
        return path
    
    def isRunning(self) -> bool:
        """Check if algorithm is still running"""
        return self.running
    
    def getName(self):
        return self.name
