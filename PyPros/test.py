import os


files=[]

for file in os.listdir():
    files.append(file)

print(files)
