list = List()
list.add("hogehoge")
list.add("fugafuga")
list.add("mogamoga")

write("0: '%s'\n", list.get(0))
write("1: '%s'\n", list.get(1))
write("2: '%s'\n", list.get(2))

write("\ndelete 1.\n\n")
list.delete(1)
write("0: '%s'\n", list.get(0))
write("1: '%s'\n", list.get(1))

value = "I love Fumie!"
write("\ninsert '%s' at index 1.\n\n", value)
list.insert(1, value)
write("0: '%s'\n", list.get(0))
write("1: '%s'\n", list.get(1))
write("2: '%s'\n", list.get(2))
