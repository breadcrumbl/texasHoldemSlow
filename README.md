# tesxasHoldemSlow

Instructions:
Feed 7 cards to CL as follows, where h indicates hole card
./main  TSh JSh QS KS AS 2C 7D

Program will calculate your best hand and your win rate (i.e. the percentage of times you win against every other
possible two card combination your oponent could hold)

TO-DO:
* Include number of oponents in calc (e.g. if win against 90% of hands but 3 oponents, win rate ~ 0.9^3)
* Calculate pot equity for flop, turn as well as river 

CL compilation shortcut:
g++ -std=c++20 main.cpp cards.cpp -o main
