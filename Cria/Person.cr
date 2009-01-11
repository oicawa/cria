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
        if .sex == true
            honorific = "Mr."
        elif .has_married == true
            honorific = "Mrs."
        else
            honorific = "Miss,"
        return honorific + .family_name


write("Hello, cria world.\n")
oikawa = Person()
oikawa.first_name = "Masamitsu"
oikawa.family_name = "Oikawa"
oikawa.sex = true
oikawa.has_married = true

watanabe = Person()
watanabe.first_name = "Fumie"
watanabe.family_name = "Watanabe"
watanabe.sex = false
watanabe.has_married = false

fumie = Person()
fumie.first_name = "Fumie"
fumie.family_name = "Oikawa"
fumie.sex = false
fumie.has_married = true

write("Your name is %s\n", oikawa.get_honorific_name())
write("Your honny is %s\n", watanabe.get_honorific_name())
write("Your wife is %s\n", fumie.get_honorific_name())
