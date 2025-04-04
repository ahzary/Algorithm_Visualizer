import logging

logging.basicConfig(level=logging.DEBUG)


class Algorithm:
    def __init__(self,map,startPoint,endPoint):
        self.map = map
        self.name = __name__
        self.stepNumber = 0
        self.nodesVisited = 0
        self.startPoint = startPoint
        self.endPoint = endPoint
        self.running = True
        print(f"Python class created: Start {startPoint}, End {endPoint}")
        self.x = 0

    def step(self):
        #do a bunch of stuff
        print ("steps taken")
        result = [[self.startPoint[0]+self.x,self.startPoint[1]+1,4],[2,2,3]]
        #self.running = False
        self.x = self.x+1
        return result
    def goalReached(self):
        return self.running


if __name__ == "__main__":
    print("Please run through Algorithm Visualizer")
