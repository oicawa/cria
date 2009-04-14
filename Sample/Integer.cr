increment(i):
    write("i = %s\n", i)
    i = i + 1
    if i < 10
        increment(i)
    return i

increment(2)

write("----------\n")
write("Input number as x. >> ")
input_x = read()
write("Input number x = [%s]\n", input_x)

write("Input number as y. >> ")
input_y = read()
write("Input number y = [%s]\n", input_y)
write("x + y = [%s]\n", input_x + input_y)


write("\nParse to integer.\n")
input_x = Integer.parse(input_x)
write("Parse number x = [%s]\n", input_x)
input_y = Integer.parse(input_y)
write("Parse number y = [%s]\n", input_y)

write("x + y = [%s]\n", input_x + input_y)

