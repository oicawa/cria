increment(value):
    if (value == 10)
        return value
    write("current value = %s\n", value)
    return increment(value + 1)

write("answer = %s\n\n", (1 + 2 - 5) * 3)

write("value = %s\n", increment(1))

