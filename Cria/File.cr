write("Read file of 'Integer.cr'\n---\n")

file = File("./Integer.cr")
file.open("r")
index = 0
while (file.is_end() == false)
    index = index + 1
    line = file.read()
    write(line)
    #write("line %s : %s", index, line)
    if (20 < index)
        break

file.close()

