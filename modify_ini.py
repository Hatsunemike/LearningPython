import os

filedir = "."
open = True
disabled_str = "DISABLED_"
len_dis_str = len(disabled_str)

if __name__ == "__main__" :
    for root, dirpaths, dirnames in os.walk(filedir) :
        for name in dirnames:
            if name.endswith(".ini") and (name.startswith("DISABLED_") == open) :
                print("detect file: "+root+os.sep+name)
                if open :
                    os.rename(root+os.sep+name, root+os.sep+name.removeprefix(disabled_str))
                else :
                    os.rename(root+os.sep+name, root+os.sep+disabled_str+name)