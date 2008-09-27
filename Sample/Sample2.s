io.write("Please input email address. > ")
input:String = io.readLine()

regex:Regex = Regex("[-A-Za-z0-9_]+@[-A-Za-z0-9_.]+", Regex.SINGLE_LINE)
fields:[]<String> = input.split(",")

if fields.Count != 2
    


if email == null
    io.writeLine("Not Email Format...(%s)", input)
else
    io.writeLine("Input email's ID is '%s', DOMAIN is '%s'.", email.getId(), email.getDomain())

io.write("Would you like to input again? (y/n) > ")
answer:String = io.read()
if answer.toLower() == "n"


io.writeLine("Good bye.")

