b = 0.3 # bto基础SB
v0 = 140 # bto基础速度

maxt = 220.0

def vn(n: int) -> float:
    return v0 * (1+b) * (6.4+0.3*n)

def tn(n: int) -> float:
    return 10000.0/vn(n)

def fan(v: float) -> float: # 风扇给的前台强度
    return v*0.002

import Muls

class FanFsBoost(Muls.BaseMulZone): # 风扇前台强度增加
    speed = 1.0
    def __init__(self, **kwargs):
        defaults = {
            "name": "fanFsBoost",
            "zoneType": "mul",
            "speed": 1.0,
            "result": 1.0
        }
        defaults.update(kwargs)
        for key, value in defaults.items():
            setattr(self, key, value)
    
    def calcResult(self):
        self.result = 1 + fan(self.speed)
    
    def getResult(self) -> float:
        return self.result

fanFsBoost = FanFsBoost(speed=v0)

class FsZone(Muls.BaseMulZone): # 前台强度乘区
    fs = 0.0
    baseFs = 100.0
    def __init__(self, **kwargs):
        defaults = {
            "name": "fsZone",
            "zoneType": "mul",
            "baseFs": 100.0,
            "result": 1.0
        }
        defaults.update(kwargs)
        for key, value in defaults.items():
            setattr(self, key, value)
    
    def calcResult(self):
        self.fs = self.baseFs
        for subzone in self.subzones :
            subzone.calcResult()
            if subzone.zoneType == "mul" :
                self.fs *= subzone.getResult()
            elif subzone.zoneType == "add" :
                self.fs += subzone.getResult()
        self.result = self.fs / 100.0
    
    def getResult(self) -> float:
        return self.result

    def getFs(self) -> float:
        return self.fs

fsZone = FsZone(subzones=[fanFsBoost])

# Jpse: 击破特攻
drillJpseZone = Muls.BaseMulZone(name="drillJpseZone", zoneType="add", val=2.5)
baseJpseZone = Muls.BaseMulZone(name="baseJpseZone", zoneType="add", val=2.0) # bto自身的击破特攻（算上阮梅、大丽花、灵砂）
jpseZone = Muls.BaseMulZone(name = "jpseZone", zoneType="mul", val=0.0, subzones=[
    baseJpseZone
])

# jpe: 击破效率
class JpeZone(Muls.BaseMulZone) :
    def calcResult(self):
        super().calcResult()
        self.result = min(3.0, self.result)

drillJpeZone = Muls.BaseMulZone(name="drillJpeZone", zoneType="add", val=0.5)
fanJpeZone = Muls.BaseMulZone(name="fanJpeZone", zoneType="add", val=0.25)
ruanmeiJpeZone = Muls.BaseMulZone(name="ruanmeiJpeZone", zoneType="add", val=0.5)
dahliaJpeZone = Muls.BaseMulZone(name="dahliaJpeZone", zoneType="add", val=0.5)
jpeZone = JpeZone(name="jpeZone", zoneType="mul", val=1.0, subzones=[
    fanJpeZone, ruanmeiJpeZone, dahliaJpeZone
])

# 总伤
btoZone = Muls.BaseMulZone(name="btoZone", val=1.0, subzones=[
    fsZone, jpseZone, jpeZone
])

if __name__ == "__main__":
    print("======== 双鞋风扇 =======")
    k = 0
    tnow = 0
    totalDmg = 0.0
    while tnow < maxt:
        tnow += tn(k)
        k += 1
        fanFsBoost.speed = vn(k)
        btoZone.calcResult()
        print("前台强度：{}, 回合总伤：{}".format(int(fsZone.getFs()), btoZone.getResult()))
        totalDmg += btoZone.getResult()
    print("行动次数：{}".format(k))
    print("总伤：", totalDmg)
    dualShoeDmg = totalDmg

    # 双钻风扇
    print("======== 双钻风扇 =======")
    k = 0
    tnow = 0
    totalDmg = 0.0
    for i in range(2):
        jpeZone.addSubzone(drillJpeZone)
        jpseZone.addSubzone(drillJpseZone)
    
    fanFsBoost.speed = vn(0)
    btoZone.calcResult()
    k = int(maxt / tn(0))
    totalDmg = k*btoZone.getResult()
    print("单回合伤害：{}".format(btoZone.getResult()))
    print("行动次数：{}".format(k))
    print("总伤：{}".format(totalDmg))
    dualDrillDmg = totalDmg

    print("========================")
    print("伤害比率（双钻/双鞋）；", dualDrillDmg/dualShoeDmg)