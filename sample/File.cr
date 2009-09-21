write("Read file of 'Integer.cr'\n---\n")

#File.open("./Integer.cr", "r")(file):
File.open("/home/masamitsu/Programing/cria/sample/test.csv", "r")(file):
    index = 0
    while (file.is_end() == false)
        index = index + 1
        line = file.read()
        write(line)


write("---------------------------------\n")
write("Write to File named 'test.txt'.\n")
write("---------------------------------\n")
File.open("test.txt", "w")(tmp):
    tmp.write("ほげほげ〜\n")
    tmp.write("ふがふが〜\n")
    tmp.write("民主党は売国党〜♪\n")


