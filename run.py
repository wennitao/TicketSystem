#!/usr/bin/python3

import os


def clean():
    os.system('bash ./clean.sh')


def clean_output():
    os.system('rm output*')


def run_basic_case(id):
    if id <= 2:
        os.system('./main < data/basic_' + str(id) + '/1.in > output.txt 2>/dev/null')
        os.system('diff output.txt data/basic_' + str(id) + '/1.out')
    if 3 <= id <= 4:
        for i in range(1, 6):
            os.system('./main < data/basic_' + str(id) + '/' + str(i) + '.in >> output' + str(i) + '.txt 2>/dev/null')
            print ('comparing output' + str (i))
            os.system('diff output' + str(i) + '.txt data/basic_' + str(id) + '/' + str(i) + '.out')
    if 5 <= id <= 6:
        for i in range(1, 11):
            os.system('./main < data/basic_' + str(id) + '/' + str(i) + '.in >> output' + str(i) + '.txt 2>/dev/null')
            os.system('diff output' + str(i) + '.txt data/basic_' + str(id) + '/' + str(i) + '.out')


def run_basic():
    clean_output()
    print('Input Basic Test ID you want to run:')
    id = int(input())
    run_basic_case(id)
    clean()


run_basic()
