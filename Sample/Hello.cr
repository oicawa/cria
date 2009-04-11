say_welcome():
    write("Welcome to cria world.\n" + "What's your name? >> ")
    
say_nice_to_meet_you(your_name):
    write("Well, Nice to meet you, %s\n", your_name)

create_name(your_name, is_male, has_married):
    if is_male == true
        long_name = "Mr." + your_name
    elif has_married == true
        long_name = "Mrs." + your_name
    else
        return "Miss," + your_name
    
    return long_name

say_welcome()
name = read()

write("Are you male? >> ")
answer = read()

if answer == "yes"

    write("Nice to meet you, %s\n\n", create_name(name, true, true))
    write("Is your wife cute? >> ")
    is_beautiful = read()
    
    while is_beautiful != "yes"
        write("Sorry, I can't hear you.\nIs your wife beautiful? >> ")
        is_beautiful = read()
    
    write("Oh, you are lucky beggar!!\n")

elif answer == "no"
    write("Did you married? >> ")
    answer = read()
    
    if answer == "yes"
        write("Nice to meet you, %s\n\n", create_name(name, false, true))
        
        is_first = true
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
        write("Nice to meet you, %s\n\n", create_name(name, false, false))
    else
        write("... OK, Nice to meet you, Miss,%s\n\n", name)

else
    say_nice_to_meet_you(name)

write("Good bye.\n")
