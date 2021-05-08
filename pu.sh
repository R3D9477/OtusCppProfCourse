#!/bin/bash

git add . ; git commit -m "`git branch -a | grep '*' | awk '{print $2}'`" ; git push
