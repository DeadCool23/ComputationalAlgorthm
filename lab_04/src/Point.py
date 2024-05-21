class Point:
    x: float
    y: float
    z: float
    weight: float
    
    def __init__(self, x, y, z, weight=1):
        self.x = x
        self.y = y
        self.z = z
        self.weight = weight
        
    def __str__(self):
        return f"{self.x} {self.y} {self.z} {self.weight}"

def get_interval_x(table):
    minim = table[0].x
    maxim = table[0].x
    for p in table:
        value = p.x
        if value > maxim:
            maxim = value
        if value < minim:
            minim = value
    return minim, maxim


def get_interval_y(table):
    minim = table[0].y
    maxim = table[0].y
    for p in table:
        value = p.y
        if value > maxim:
            maxim = value
        if value < minim:
            minim = value
    return minim, maxim
