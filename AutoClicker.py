import pyautogui

def readans() -> bool :
    while(1) :
        ans = input("是否继续？(y/n)")
        if ans == "y" :
            return True
        elif ans == "n":
            return False
        else :
            print("请输入正确的值!")
            continue

if __name__ == "__main__":
    while(1) :
        for i in range(100):pyautogui.click()
        ans = True
        # ans = readans()
        if not ans: exit(0)