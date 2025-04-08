import heapq
import math
from typing import List, Tuple, Dict, Optional

class Algorithm:
    def __init__(self, map: List[List[int]], startPoint: List[int], endPoint: List[int]):
        self.map = map
        self.start = tuple(startPoint)
        self.end = tuple(endPoint)
        self.open_set = []
        self.came_from: Dict[Tuple[int, int], Optional[Tuple[int, int]]] = {}
        self.g_score: Dict[Tuple[int, int], float] = {self.start: 0}
        self.f_score: Dict[Tuple[int, int], float] = {self.start: self.heuristic(self.start)}
        self.current = self.start
        self.running = True
        self.name = __name__
        
        # Initialize open set with start node
        heapq.heappush(self.open_set, (self.f_score[self.start], self.start))
        
    def heuristic(self, pos: Tuple[int, int]) -> float:
        """Euclidean distance heuristic"""
        dx = pos[0] - self.end[0]
        dy = pos[1] - self.end[1]
        return math.sqrt(dx*dx + dy*dy)
    
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
        """Execute one step of A* algorithm"""
        if not self.open_set or not self.running:
            self.running = False
            return []
        
        # Get node with lowest f_score
        current_f, current = heapq.heappop(self.open_set)
        self.current = current
        
        # Check if we've reached the goal
        if current == self.end:
            self.running = False
            return self.reconstruct_path()
        
        # Explore neighbors
        changes = []
        for neighbor in self.get_neighbors(current):
            # Tentative g_score
            tentative_g = self.g_score[current] + 1  # 1 is distance between nodes
            
            if neighbor not in self.g_score or tentative_g < self.g_score[neighbor]:
                # This path to neighbor is better than any previous one
                self.came_from[neighbor] = current
                self.g_score[neighbor] = tentative_g
                self.f_score[neighbor] = tentative_g + self.heuristic(neighbor)
                
                if neighbor not in [i[1] for i in self.open_set]:
                    heapq.heappush(self.open_set, (self.f_score[neighbor], neighbor))
                
                # Mark as explored (type 7, yellow)
                if self.map[neighbor[0]][neighbor[1]] != 6:  # Don't override end square
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
    
    def goalReached(self) -> bool:
        """Check if algorithm is still running"""
        return self.running
        
    def getName(self):
        return self.name 