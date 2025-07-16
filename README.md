# tesxasHoldemSlow

Instructions:
Feed 7 cards to CL as follows, where h indicates hole card
./main  TSh JSh QS KS AS 2C 7D

Program will calculate your best hand and your win rate (i.e. the percentage of times you win against every other
possible two card combination your oponent could hold)

CL compilation shortcut:
g++ -std=c++20 main.cpp cards.cpp -o main


Scalability Problem - At the River:
One opponent:
For one opponent (P1 - you are P0), there are 45C2 (990) combinations of 2 cards they can have.
Two opponents:
P1 has 45C2 and P2 43C2 cominations - 45C2 * 43C2 (990 * 903) combinations to consider.
Generally N opponents:
Every Nth player added can have (45 - 2(N-1))C2 combinations of cards.
Total combinations to consider: ( 45! - (45-2(N-1))! ) / 2^N
e.g. for 10 players the total combinations to consider are on the order of 10^53 (require a supercomputer)
