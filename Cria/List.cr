print_list_contents(list):
    count = list.count()
    i = 0
    while i < count
        write("%s: '%s'\n", i, list.get(i))
        i = i + 1
    write("\n")


list = List()

write("add items to list.\n\n")
list.add("hogehoge")
list.add("fugafuga")
list.add("mogamoga")
print_list_contents(list)

write("delete 1.\n\n")
list.delete(1)
print_list_contents(list)

value = "I love Fumie!"
write("insert '%s' at index 1.\n\n", value)
list.insert(1, value)
print_list_contents(list)

    
        


