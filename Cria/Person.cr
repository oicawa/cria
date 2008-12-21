Person:
    @name:
    @is_male:
    @has_married:
    mr:
    mrs:
    miss:
    
    new():
    
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