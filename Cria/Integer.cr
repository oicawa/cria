increment(i):
    write("i = %s\n", i)
    i = i + 1
    if i < 10
        increment(i)
    return i

increment(2)