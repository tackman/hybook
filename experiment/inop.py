s = 'Some string'
x = [1, 2, 3]
y = [0, 4, 5]

a = s in x in y
print(a)  # False

b = (s in x) in y
print(b)  # True

def f():
    print('f() evaluated')
    return [1, 2, 3]

def g():
    print('g() evaluated')
    return [0, 4, 5]

c = s in f() in g()
print(c)
# f() evaluated
# False

d = (s in f()) in g()
print(d)
# f() evaluated
# g() evaluated
# True
