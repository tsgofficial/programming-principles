def foo(y=None):
    global X
    if y is None:
        X *= 2
    else:
        X += y


X = 2

foo(3)   # like foo(Y)
foo()    # like foo()

print(X)