from typing import List

class Algorithm:
    def __init__(self, map: List[List[int]], startPoint: List[int], endPoint: List[int]):
        self.map = map
        self.start = startPoint
        self.end = endPoint
        self.running = True  # Must be managed by your algorithm

    def step(self) -> List[List[int]]:
        """Required: Return list of [x, y, tile_type] updates"""
        changes = []
        
        # Your algorithm logic here
        # Update self.running when done
        # Append changes like: changes.append([x, y, 3])  # 3=explored
        
        return changes

    def isRunning(self) -> bool:
        """Required: Return False when algorithm finishes"""
        return self.running
        
    def getName(self):
        return self.name 
        