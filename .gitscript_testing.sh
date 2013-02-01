#!/bin/bash 

git add . -v
echo -n "Comment: "
read comment
git commit -a -v -m "$comment"
git push -v
