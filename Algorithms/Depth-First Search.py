from typing import List, Tuple, Dict, Optional

class Algorithm:
    def __init__(self, map: List[List[int]], startPoint: List[int], endPoint: List[int]):
        self.map = map
        self.start = tuple(startPoint)
        self.end = tuple(endPoint)
        self.stack = [self.start]
        self.visited = set([self.start])
        self.came_from: Dict[Tuple[int, int], Optional[Tuple[int, int]]] = {self.start: None}
        self.current = self.start
        self.running = True
        self.name = __name__

    def get_neighbors(self, pos: Tuple[int, int]) -> List[Tuple[int, int]]:
        """Get walkable neighbors (4-directional)."""
        directions = [(0, 1), (1, 0), (0, -1), (-1, 0)]
        neighbors = []
        x, y = pos
        for dx, dy in directions:
            nx, ny = x + dx, y + dy
            if 0 <= nx < len(self.map) and 0 <= ny < len(self.map[0]):
                if self.map[nx][ny] in [1, 6]:  # 1 (walkable), 6 (end)
                    neighbors.append((nx, ny))
        return neighbors

    def step(self) -> List[List[int]]:
        """Execute one step of DFS (process one node and its neighbors)."""
        if not self.stack or not self.running:
            self.running = False
            return []

        current = self.stack.pop()
        self.current = current

        # Check if goal is reached
        if current == self.end:
            self.running = False
            return self.reconstruct_path()

        changes = []
        for neighbor in self.get_neighbors(current):
            if neighbor not in self.visited:
                self.visited.add(neighbor)
                self.came_from[neighbor] = current
                self.stack.append(neighbor)
                if neighbor != self.end:
                    changes.append([neighbor[0], neighbor[1], 7])  # Yellow for explored

        changes.append([current[0], current[1], 3])  # Red for current
        return changes

    def reconstruct_path(self) -> List[List[int]]:
        """Reconstruct the path (if found)."""
        if self.end not in self.came_from:
            return []
        path = []
        current = self.end
        while current in self.came_from:
            path.append([current[0], current[1], 2])  # Blue for path
            current = self.came_from[current]
        path.append([self.start[0], self.start[1], 2])
        return path

    def isRunning(self) -> bool:
        return self.running

    def getName(self):
        return self.name