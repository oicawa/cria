write("Welcome to cria world.\n" + "What's your name? >> ")
name = read()

write("Are you male? >> ")
answer = read()

if answer == "yes"
    write("Nice to meet you, Mr.%s\n\n", name)
    write("Is your wife beautiful? >> ")
    isBeautiful = read()
    while isBeautiful != "yes"
        write("Really? I ask you same question again.\nIs your wife beautiful? >> ")
        isBeautiful = read()
    write("Oh, you are lucky beggar!!\n")
elif answer == "no"
    write("Did you married? >> ")
    answer = read()
    if answer == "yes"
        write("Nice to meet you, Mrs,%s\n\n", name)
    elif answer == "no"
        write("Nice to meet you, Miss,%s\n\n", name)
    else
        write("... OK, Nice to meet you, Miss,%s\n\n", name)
else
    write("I can not understand about '%s'.\n\nWell, Nice to meet you, %s\n\n", answer, name)

write("Good bye.\n")
