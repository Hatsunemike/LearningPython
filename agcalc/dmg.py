from .Muls import *

class Dmg:
    muls: list[BaseMulZone] = []
    def __init__(self) -> None:
        self.muls.append(MulZone(val=100.0))
        self.muls.append(MulZone(val=1.0)) # 倍率
        self.muls.append(CritZone(critRate=0.1, critDmg=1)) # 双暴
    
    def getDmg(self) -> float:
        if len(self.muls) == 0: return 0.0
        dmgVal = self.muls[0].getResult()
        for zone in self.muls[1:]:
            if zone.zoneType == "mul": dmgVal *= zone.getResult()
            elif zone.zoneType == "add": dmgVal += zone.getResult()
        return dmgVal