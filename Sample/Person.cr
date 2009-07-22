Person:
    @first_name
    @family_name
    @sex
    @has_married
    @birthday
    MR = "Mr."
    MRS = "Mrs."
    MISS = "Miss,"
    test = "hogehoge"
    
    
    new(first_name, family_name):
        .first_name = first_name
        .family_name = family_name
    
    get_all_honorific(message):
        return class.MR + "/" + class.MRS + "/" + _
               Person.MISS + "(" + message + ")"
    
    #is_male(person):
    #    return person.sex
    
    @get_honorific_name():
        if .sex == true
            honorific = class.MR
        elif .has_married == true
            honorific = class.MRS
        else
            honorific = class.MISS
        return honorific + .family_name + "(" + .get_full_name() + ")"
    
    @get_full_name():
        return .first_name + "-" + .family_name
    
    @write_all_honorific(message):
        write("Test all honorific -> '%s'\n", class.get_all_honorific(message))

marry(man, woman):
    if man.sex != true
        write("'%s' is not male.\n", man.get_full_name())
        return
    if woman.sex != false
        write("'%s' is not female.\n", woman.get_full_name())
        return
    elif man.has_married != false
        write("'%s' has already married.\n", man.get_full_name())
        return
    elif woman.has_married != false
        write("'%s' has already married.\n", woman.get_full_name())
        return
    
    write("Man's family name is '%s'\n", man.family_name)
    write("Woman's family name is '%s'\n", woman.family_name)
    write("Marry!\n")
    woman.family_name = man.family_name
    woman.has_married = true
    write("Woman's new family name is '%s'\n", woman.family_name)


write("Hello, cria world.\n")
oikawa = Person("Masamitsu", "Oikawa")
oikawa.sex = true
oikawa.has_married = false

watanabe = Person("Fumie", "Watanabe")
watanabe.sex = false
watanabe.has_married = false

write("Your name is %s\n", oikawa.get_honorific_name())
write("Your honny is %s\n", watanabe.get_honorific_name())


marry(oikawa, watanabe)

write("Your wife is %s\n", watanabe.get_honorific_name())

write("Person.MR   = '%s'\n", Person.MR)
write("Person.MRS  = '%s'\n", Person.MRS)
write("Person.MISS = '%s'\n", Person.MISS)

write("All Honorifics = '%s'\n", Person.get_all_honorific("!!!hoge,fuga!!!"))

write("Test static field 'test' = '%s'\n", Person.test)
Person.test = "fugafuga"
write("Test again static field 'test' = '%s'\n", Person.test)

oikawa.write_all_honorific("!!mogamoga!!")

