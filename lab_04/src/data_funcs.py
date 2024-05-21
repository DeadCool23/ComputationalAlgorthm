from src.Point import Point

def fread_2Ddata(path: str):
    with open(path) as f:
        data = []
        for line in f:
            d = [float(i) for i in line.split()]
            data.append(Point(d[0], d[1], 0, d[2]))

    return data

def fread_3Ddata(path: str):
    with open(path) as f:
        data = []
        flag = False
        for line in f:
            if line[0] == '/':
                flag = True
                continue
            d = [float(i) for i in line.split()]
            if flag:
                data.append(Point(d[1], d[2], d[0], d[3]))
            else:
                data.append(Point(d[0], d[1], d[2], d[3]))

    return data

def set_all_weight(data, w=1):
    for d in data:
        d.weight = w
        
def set_n_weight(data, n, w=1):
    if 0 <= n < len(data):
        data[n].weight = w

def print_data(data):
    i = 0
    for d in data:
        print(f"{i})", d)
        i += 1
