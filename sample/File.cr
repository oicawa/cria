write("Read file of 'Integer.cr'\n---\n")

#File.open("./Integer.cr", "r")(file):
File.open("/home/masamitsu/Programing/cria/Sample/Integer.cr", "r")(file):
    index = 0
    while (file.is_end() == false)
        index = index + 1
        line = file.read()
        write(line)


write("---------------------------------\n")
write("Write to File named 'test.txt'.\n")
write("---------------------------------\n")
File.open("test.txt", "w")(tmp):
    tmp.write("write test!\n")
    tmp.write("write test2!\n")
    tmp.write("write test3!\n")


