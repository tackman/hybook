def tarai(x, y, z):
    return y if x <= y else tarai(tarai(x - 1, y, z), tarai(y - 1, z, x),
        tarai(z - 1, x, y))


import time
start = time.perf_counter()
print('begin takeuchi benchmark...')
print(tarai(14, 6, 0))
print('take time ', time.perf_counter() - start)

