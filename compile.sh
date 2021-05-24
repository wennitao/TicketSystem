rm *.dat
sleep 0.5s
g++ -o main main.cpp -std=c++11 -fsanitize=address -fsanitize=undefined