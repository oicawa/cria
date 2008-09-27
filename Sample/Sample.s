File
    @filePath:String
    @openMode:OpenMode
    @accessMode:
    
    @file:NativeFile
    
    new(filePath:String, openMode:OpenMode)
        .filePath = filePath
        .openMode = openMode
        
    @open(mode:OpenMode)
        file = File_open(.filePath, getNativeOpenMode(mode))
    
    #getNativeOpenMode(mode:OpenMode):String
        if mode == OpenMode.READ
            return "r"
        elif mode == OpenMode.WRITE
            return "w"
        elif mode == OpenMode.APPEND
            return "a"
        else
            throw Exception("'%s' is not OpenMode.".set(mode))
            
    exist(filePath:String):Boolean
    
    do(filePath:String, openMode:OpenMode)|file:File|
        if File.exist(filePath) == false
            throw FileNotFoundException()
        
        file = .open(filePath, openMode)
        
        call!
        
        finally!
            file.close()
        
        
Button -> Control
    @caption:String
    @parent:Control
    
    @mouseUp:CallBack(event:MouseUpEvent, sender:Control)
    
    new(parent:Control)
        .parent = parent
    
InputFrame -> Frame
    @layout0:TableLayout
    
    @layout1:TableLayout
    @nameLabel:Label
    @nameTextBox:TextBox
    @emailLabel:Label
    @emailTextBox:TextBox
    @postalLabel:TextBox
    @postalTextBox:TextBox
    
    @layout2:TableLayout
    @okButton:Button
    @cancelButton:Button
    
    new(parent:Control)
        .layout0 = TableLayout(this)
        .layout0.column = 1
        .layout0.row = 2
        
        .layout1 = TableLayout(.layout0)
        .layout1.column = 2
        .layout1.row = 3
        
        .nameLabel = Label(.layout1)
        .nameLabel.caption = "Name"
        .nameTextBox = TextBox(.layout1)
        .emailLabel = 
    
MainFrame -> Frame
    @layout:TableLayout
    @grid:Grid
    @addButton:Button
    @editButton:Button
    @deleteButton:Button
    
    
    new(parent:Control)
        .parent = parent
        
        .layout = TableLayout(this)
        .layout.column = 3
        .layout.row = 2
        
        .addressLabel = Label(.layout)
        .addressLabel.caption = "Input Email Address:"
        
        .addressTextBox = TextBox(.layout)

        .addButton = Button(.layout)
        .addButton.caption = "Add"
        .addButton.mouseUp = CallBack(event:MouseUpEvent, sender:Control)
        
        .deleteButton.mouseUp = mouseUpDeleteButton(event, sender)
                             
        .layout.put(0, 0, .addressLabel)
        .layout.put(1, 0, .addressTextBox)
        .layout.put(1, 1, .checkButton)
        
    @mouseUpAddButton(event:MouseUpEvent, sender:Control)
        email:Email = Email.parse(textBox.text)
        .showMessage("Parse OK.\n\n ID     : %s\n DOMAIN : %s".set(email.getId(), email.getDomain()), _
                     "Result", _
                     Icon.INFORMATION, _
                     Button.OK)
        
    @mouseUpDeleteButton(event:MouseUpEvent, sender:Control)
        DialogResult result = .showMessage("Can I delete it?", _
                                           "Confirm", _
                                           Icon.QUESTION, _
                                           Button.OK|Button.CANCEL, _
                                           Button.OK)
        if result == DialogResult.OK
            this.close()

    @shown()
        File.do('./data.txt', OpenMode.READ)|file:File|
            entries:[]<Entry> = []<Entry>
            file.getAllLine().each()|line:String|
                entry:Entry = Entry()
                fields:[]<String> = line.split(",")
                entry.name = field[0]
                entry.email = Email.parse(field[1])
                entry.postal = field[2]
                catch! ex
                    logger.debug(ex.message)
                entries.add(entry)
        
        

Email
    ID:String = "ID"
    DOMAIN:String = "DOMAIN"
    #regex:Regex
    
    #@email:String
    #@match:Match
    
    #new(email:String)
        .email = email
        .match = .regex.getMatch(.email)
    
    static()
        .regex = Regex("^(?<" + .ID + ">[A-Za-z0-9_-.]+?)@(?<" + .DOMAIN + >[A-Za-z0-9_-.]+?)$", Regex.SINGLE_LINE)
        
    parse(email:String):Email
        if .regex.isMatch(email) == false
            return null
        return Email(email)
    
    @getId():String
        return .match.group[.ID]

    @getDomain():String
        return .match.group[.DOMAIN]

    @toString():String
        return .email


main(args:[]<String>)
    while true
        io.write("Please input email address. > ")
        input:String = io.readLine()
        email:Email = Email.parse(input)
        if email == null
            io.writeLine("Not Email Format...(%s)", input)
        else
            io.writeLine("Input email's ID is '%s', DOMAIN is '%s'.", email.getId(), email.getDomain())

        io.write("Would you like to input again? (y/n) > ")
        answer:String = io.read()
        if answer.toLower() == "n"
            break;


    io.writeLine("Good bye.")

