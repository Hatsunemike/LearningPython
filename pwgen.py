import sys

help_str = "If you want to get the instruction of cmd-arguments,\n\
please type \"python {} -h\" for help.".format(sys.argv[0])

def ShowHelpInfo() :
    print("The types of arguments and their usage:\n\
        -h      for help\n\
        -l      tell the length of generated password\n\
        -n      the password will include numbers.(if no content-indicator, this will open)\n\
        -a      the password will include lowercase.\n\
        -A      the password will include uppercase.\n\
        -s      the password will include special characters, such as ~!@#$%")

sp_char_list = ['~','!','@','#','$','%','^','&','*','+','(',')'] # 特殊符号集合
number_list = [str(x) for x in range(10)] # 数字集合
low_eng_list = [chr(ord('a')+x) for x in range(26)] # 小写英文字母
upp_eng_list = [chr(ord('A')+x) for x in range(26)] # 大写英文字母

''' The arguments transported by command line. '''
length = 12 # 长度
with_sp_char = False # 是否带特殊字符
with_eng = 0 # 是否带英文字母（不带为0，带小写为1，大写为2，都要为3）
with_number = False # 是否带数字

def sperate_args(argv:list) -> set :
    '''作用：对命令行传来的参数进行词法分析，形成{(name,(value)),(name,(value),...),...}的参数形式'''
    argv.append("")
    arg_set = set()
    arg_li = list()
    for arg in argv[1:]:
        if(arg == '' or arg[0] == '-'):
            if(len(arg_li) != 0) : arg_set.add(tuple(arg_li))
            arg_li = [arg]
        else :
            try:
                val = int(arg)
            except: 
                print("This argument \"{}\" is not number or arg.".format(arg)+help_str)
                exit(0)
            if (len(arg_li) == 0) :
                print("The arg-number {} cannot be interpreted, type a arg-section before it .\n"+help_str)
                exit(0)
            arg_li.append(val)
    return arg_set

def TurnArgiStr(arg: tuple):
    if arg.__len__() == 1: return arg[0]
    else:
        ans = arg[0]
        for x in arg[1:] : ans += x
        return ans

def argScaleException(wrong_arg: tuple) :
    print("There exists redundant numbers in argument \"{}\".".format(TurnArgiStr(wrong_arg)))
    exit(0)

def process_argv(argv: list) :
    ''' 
    format of arg_set:{arg1,arg2,arg3,...argi,...}
        → format of argi(tuple):[name,(value)]
            → name, value are both str. Can be in lack of value.
    '''
    global with_number
    global with_eng
    global with_sp_char
    global length
    arg_set = sperate_args(argv)
    for arg in arg_set:
        if arg[0] == '-h' : # 显示帮助
            if len(arg_set) > 1: # 除了-h还有别的参数
                print("Arguments Wrong. \n" + help_str)
                exit(0)
            if len(arg)>1:argScaleException(arg)
            ShowHelpInfo()
            exit(0)
        elif arg[0] == '-A' : # 含大写
            if len(arg)>1:argScaleException(arg)
            with_eng = with_eng | 2
        elif arg[0] == '-a' : # 含小写
            if len(arg)>1:argScaleException(arg)
            with_eng = with_eng | 1
        elif arg[0] == '-n' : # 含数字
            if len(arg)>1:argScaleException(arg)
            with_number = True
        elif arg[0] == '-s' : # 含特殊字符
            if len(arg)>1:argScaleException(arg)
            with_sp_char = True
        elif arg[0] == '-l' : # 设置长度
            if len(arg)>2:argScaleException(arg)
            length = arg[1]
            if length < 0 or length > 512:
                print("Please input a write length!\n")
                exit(0)
        else: # 不合法参数
            print("Illegal argument:",TurnArgiStr(arg))
            exit(0)

def printSettings() :
    '''输出参数'''
    print("length:",length)
    print("with_sp_char:",with_sp_char)
    print("with_number:",with_number)
    if with_eng == 0 : engstr = "none"
    elif with_eng == 1 : engstr = "lower"
    elif with_eng == 2 : engstr = "upper"
    elif with_eng == 3 : engstr = "lower and upper"
    else : engstr = "unknown"
    print("with_letters:",engstr)

import random

def generatePassword():
    global with_number
    global with_eng
    global with_sp_char
    global length
    if not (with_sp_char | with_number | with_eng) :
        with_number = True
    printSettings()
    charset = []
    if (with_eng | 1) : charset += low_eng_list
    if (with_eng | 2) : charset += upp_eng_list
    if with_sp_char :   charset += sp_char_list
    if with_number :    charset += number_list
    # print("charset:",charset)
    tmpstr = random.sample(charset, length)
    ansstr = ""
    for char in tmpstr: ansstr += char
    return ansstr

if __name__ == "__main__" :
    process_argv(sys.argv)
    print("=========== Password Generation ==========")
    ansstr = generatePassword()
    print("=========== Generation Ends ==============")
    print("Generated Password:",ansstr)