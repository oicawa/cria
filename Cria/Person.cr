Person:
    @first_name:
    @family_name:
    @sex:
    @age:
    @birthday:
    mr:
    mrs:
    miss:
    
    @get_honorific_name():
        honorific = ""
        if is_male == true
            honorific = "Mr."
        elif has_married == true
            honorific = "Mrs."
        else
            honorific = "Miss,"
        return honorific + .name


person = Person()

write("Input your first name.  >> ")
person.first_name = read()

write("Input your family name. >> ")
person.family_name = read()

write("Input your sex.         >> ")
person.sex = read()

write("Input your age.         >> ")
person.age = read()

write("Input your birthday.    >> ")
person.birthday = read()

write("%s-%s\n", person.first_name, person.family_name)
write("  sex     : %s\n", person.sex)
write("  age     : %s\n", person.age)
write("  birthday: %s\n", person.birthday)
