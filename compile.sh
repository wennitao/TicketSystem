rm *.dat
sleep 0.5s
g++ main.cpp -o main -O2 -std=c++11 -fsanitize=address -fsanitize=undefined