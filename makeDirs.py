import os
import string
import random

def makeFiles(path):
    os.makedirs(path, exist_ok=True)
    os.chdir(path)
    for i in range(100000):
        filename = f"test{i+1}.txt"
        with open(filename, "w") as file:
            random_string = ''.join(random.choices(string.ascii_uppercase + string.digits, k=100))
            file.write(random_string)

if __name__ == "__main__":
    makeFiles("test")