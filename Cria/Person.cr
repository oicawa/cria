Person:
    @first_name:
    @family_name:
    @sex:
    @has_married:
    @birthday:
    mr:
    mrs:
    miss:
    
    @get_honorific_name():
        #hoge = false
        #write("hoge = %s\n", hoge)
        write(".sex = %s\n", .sex)
        if .sex == true
            honorific = "Mr."
        elif .has_married == true
            honorific = "Mrs."
        else
            honorific = "Miss,"
        return honorific + .family_name


person = Person()

write("Input your first name.  >> ")
person.first_name = read()

write("Input your family name. >> ")
person.family_name = read()

write("Are you male? (y/n)     >> ")
answer = read()
if answer == "y"
    person.sex = true
else
    person.sex = false

write("Have you married.(y/n)  >> ")
answer = read()
if answer == "y"
    person.has_married = true
else
    person.has_married = false

#write("Input your birthday.    >> ")
#person.birthday = read()

#person.hogehoge = "fugafuga"
write("Family Name is member field? (family_name='%s')\n", person.family_name)
#write("Family Name is global variable? (family_name='%s')\n", family_name)
write("Hello, %s!\n", person.get_honorific_name())
#write("%s-%s\n", person.first_name, person.family_name)
#write("  sex     : %s\n", person.sex)
#write("  age     : %s\n", person.age)
#write("  birthday: %s\n", person.birthday)
