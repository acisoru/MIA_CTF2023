#!/bin/bash
echo hi..
PYTHONUNBUFFERED=1 PYTHONPATH=/Lib /python -c 'while True: print(eval(input()))'
