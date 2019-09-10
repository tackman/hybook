class A:
    def __init__(self):
        self.x = 0

    def f(self):
        print('A.f ', self.x)
        self.x = self.x + 1
        return self

    def g(self, n):
        print('A.g', self.x)
        self.x = self.x + n
        return self

    def h(self, n, m):
        print('A.h', self.x)
        self.x = self.x + n + m
        return self


