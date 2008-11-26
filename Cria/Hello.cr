sayWelcome():
    write("Welcome to cria world.\n" + "What's your name? >> ")
    
sayNiceToMeetYou(yourName):
    write("Well, Nice to meet you, %s\n", yourName)

createName(yourName, isMale, hasMarried):
    if isMale == true
        return "Mr." + yourName
    elif hasMarried == true
        return "Mrs." + yourName
    else
        return "Miss," + yourName

sayWelcome()
name = read()

write("Are you male? >> ")
answer = read()

if answer == "yes"

    write("Nice to meet you, %s\n\n", createName(name, true, true))
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
        write("Nice to meet you, %s\n\n", createName(name, false, true))
        
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
        write("Nice to meet you, %s\n\n", createName(name, false, false))
    else
        write("... OK, Nice to meet you, Miss,%s\n\n", name)

else
    sayNiceToMeetYou(name)

write("Good bye.\n")
