from collections import deque
from typing import List, Tuple, Dict, Optional

class Algorithm:
    def __init__(self, map: List[List[int]], startPoint: List[int], endPoint: List[int]):
        self.map = map
        self.start = tuple(startPoint)
        self.end = tuple(endPoint)
        self.queue = deque()
        self.came_from: Dict[Tuple[int, int], Optional[Tuple[int, int]]] = {}
        self.visited = set()
        self.current = self.start
        self.running = True
        self.name = __name__

        # Initialize queue with start node
        self.queue.append(self.start)
        self.visited.add(self.start)
        self.came_from[self.start] = None
    
    def get_neighbors(self, pos: Tuple[int, int]) -> List[Tuple[int, int]]:
        """Get walkable neighbors (4-directional)"""
        directions = [(0, 1), (1, 0), (0, -1), (-1, 0)]
        neighbors = []
        x, y = pos
        
        for dx, dy in directions:
            nx, ny = x + dx, y + dy
            if (0 <= nx < len(self.map)) and (0 <= ny < len(self.map[0])):
                if self.map[nx][ny] in [1, 6]:  # 1 (walkable), 6 (end square)
                    neighbors.append((nx, ny))
        return neighbors
    
    def step(self) -> List[List[int]]:
        """Execute one step of BFS, processing ALL nodes at the current depth level"""
        if not self.queue or not self.running:
            self.running = False
            return []

        changes = []
        # Process all nodes at the current depth level
        current_level_size = len(self.queue)
        for _ in range(current_level_size):
            current = self.queue.popleft()
            self.current = current

            # Check if we've reached the goal
            if current == self.end:
                self.running = False
                return self.reconstruct_path()

            # Explore neighbors
            for neighbor in self.get_neighbors(current):
                if neighbor not in self.visited:
                    self.visited.add(neighbor)
                    self.came_from[neighbor] = current
                    self.queue.append(neighbor)

                    # Mark as explored (type 7, yellow)
                    if self.map[neighbor[0]][neighbor[1]] != 6:
                        changes.append([neighbor[0], neighbor[1], 7])

            # Mark current position (type 3, red)
            changes.append([current[0], current[1], 3])
        
        return changes
    
    def reconstruct_path(self) -> List[List[int]]:
        """Reconstruct the final path (same as Dijkstra's)"""
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