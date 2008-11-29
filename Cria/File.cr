file = File("./Integer.cr")
file.open()
index = 0
while (file.isEnd() == false)
    index = index + 1
    line = file.read()
    write("line %s : %s", index, line)
    if (20 < index)
        break

file.close()

