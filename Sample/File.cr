write("Read file of 'Integer.cr'\n---\n")

#File.open("./Integer.cr", "r")(file):
File.open("/home/masamitsu/Programing/cria/Sample/Integer.cr", "r")(file):
    index = 0
    while (file.is_end() == false)
        index = index + 1
        line = file.read()
        write(line)


#file = File("test.txt")
#file.open("w")
#file.write("write test!\n")
#file.write("write test2!\n")
#file.write("write test3!\n")
#file.close()


