import os
import glob
import argparse

def formatFile(file):
    print(file)
    os.system('clang-format -i -style=LLVM ' + file)

def fileInList(file, list):
    for elem in list:
        if elem in file:
            return True
    return False

parser = argparse.ArgumentParser(description='Format cpp files in folder')
parser.add_argument('folder', nargs='?', default = './', help='Where to search for files')
parser.add_argument('-apart', nargs='*', help='Folders that must be not formatted')
parser.add_argument('-extensions', nargs='*', help='Extensions that must be procceded')
args = parser.parse_args()

extensions = ['cpp', 'hpp', 'c', 'h']
extra_files = args.apart # can be extended

os.chdir(args.folder)
print("Formated files in {}:".format(os.path.abspath('./')))
for extension in extensions:
    for file in glob.glob(os.curdir + '/**/*.' + extension, recursive=True):
        if not extra_files or not fileInList(file, extra_files):
            formatFile(file)