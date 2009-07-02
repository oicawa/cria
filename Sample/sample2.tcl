#!/usr/bin/wish

label .user_label -text "申請者"
grid .user_label -column 0 -row 0 -sticky w

frame .user_frame
grid .user_frame -column 1 -row 0 -sticky ew

label .user_frame.id_label -text "社員番号"
grid .user_frame.id_label -column 0 -row 0 -sticky w

entry .user_frame.id -width 4 -background white -selectbackground blue
grid .user_frame.id -column 1 -row 1 -sticky w

label .user_frame.name_label -text "氏名"
grid .user_frame.name_label -column 2 -row 2 -sticky w

entry .user_frame.name -width 10 -background white -selectbackground blue
grid .user_frame.name -column 3 -row 3 -sticky w

button .user_frame.user_search -text "一覧" -command { .user_frame.user_search configure -text "いちらん" }
grid .user_frame.user_search -column 4 -row 4 -sticky w



label .term_label -text "期間"
frame .term_frame
entry .term_frame.from -background white -width 10 -selectbackground blue
label .term_frame.childa -text "〜"
entry .term_frame.to -background white -width 10 -selectbackground blue

grid .term_frame.from -column 0 -row 0 -sticky w
grid .term_frame.childa -column 1 -row 0 -sticky w
grid .term_frame.to -column 3 -row 0 -sticky w



label .division_label -text "区分"
frame .divisions
radiobutton .divisions.radio1 -text "有休" -variable division -value "有給休暇"
radiobutton .divisions.radio2 -text "傷休" -variable division -value "傷病休暇"
radiobutton .divisions.radio3 -text "特別" -variable division -value "特別休暇"

grid .divisions.radio1 -column 1 -row 0 -sticky w
grid .divisions.radio2 -column 2 -row 0 -sticky w
grid .divisions.radio2 -column 3 -row 0 -sticky w



label .reason_label -text "事由"
text .reason -width 5 -height 4 -background white

button .approving -text "承認依頼"


#wm geometry . 310x200
wm title . "休暇届"


grid .term_label -column 0 -row 1 -sticky w
grid .term_frame -column 1 -row 1 -sticky ew

grid .division_label -column 0 -row 2 -sticky w
grid .divisions -column 1 -row 2 -sticky w

grid .reason_label -column 0 -row 3 -sticky w
grid .reason -column 1 -row 3 -sticky ew

grid .approving -column 1 -row 4 -sticky e


