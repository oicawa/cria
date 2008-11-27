file = File("c:\test.csv")
index = 0
while file.isEnd() == false
    index = index + 1
    line = file.read()
    write("line %s : %s\n", index, line)
file.close()
