rm *.dat
sleep 0.5s
g++ main.cpp -o code -std=c++11 -fsanitize=address -fsanitize=undefined