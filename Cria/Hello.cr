write("Welcome to cria world.\n" + "What's your name? >> ")
name = read()

write("Are you male? >> ")
answer = read()

if answer == "yes"

    write("Nice to meet you, Mr.%s\n\n", name)
    write("Is your wife beautiful? >> ")
    isBeautiful = read()
    
    while isBeautiful != "yes"
        write("Sorry, I can't hear you.\nIs your wife beautiful? >> ")
        isBeautiful = read()
    
    write("Oh, you are lucky beggar!!\n")

elif answer == "no"
    write("Did you married? >> ")
    answer = read()
    
    if answer == "yes"
        write("Nice to meet you, Mrs,%s\n\n", name)
        
        isFirst = true
        while true
            write("Is your husband gentle? >> ")
            answer = read()
            if answer == "yes"
                write("Oh, you are lucky beggar, Mrs.%s!!\n", name)
                break
            else
                write("Sorry, I can't hear you.\n")
                continue
            write("\n<<continue test>>\n")
            
    elif answer == "no"
        write("Nice to meet you, Miss,%s\n\n", name)
    else
        write("... OK, Nice to meet you, Miss,%s\n\n", name)

else
    write("I can not understand about '%s'.\n\nWell, Nice to meet you, %s\n\n", answer, name)


write("Good bye.\n")
