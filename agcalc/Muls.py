class BaseMulZone:
    name = ""
    zoneType = "mul"
    val = 1 # 不考虑子乘区的结果
    result = 1 # 考虑子乘区的结果
    subzones: list = []
    def __init__(self, **kwargs):
        # 设置默认字典
        defaults = {
            "zoneType": "mul",
            "val": 1.0,
            "result": 1.0
        }
        # 用传入的参数覆盖默认值
        defaults.update(kwargs)
        
        # 批量赋值到实例属性
        for key, value in defaults.items():
            setattr(self, key, value)

    def getResult(self) -> float:
        return self.result
    
    def calcResult(self) :
        self.result = self.val
        for subzone in self.subzones :
            if subzone.zoneType == "mul" :
                subzone.calcResult()
                self.result *= subzone.getResult()
            elif subzone.zoneType == "add" :
                subzone.calcResult()
                self.result += subzone.getResult()
            else:
                raise ValueError("Invalid subzoneType: {}".format(subzone.zoneType))
    
    def addSubzone(self, subzone) :
        self.subzones.append(subzone)
    
    def clearSubzones(self) :
        self.subzones = []
    
    # Array Operators
    def __getitem__(self, key) :
        if isinstance(key, int) :
            return self.subzones[key]
        elif isinstance(key, str) :
            return next((z for z in self.subzones if z.name == key), None)
        else:
            raise ValueError("key must be a string or integer. key:", key)
    
    def __setitem__(self, key, value) :
        if isinstance(key, int) :
            self.subzones[key] = value
        elif isinstance(key, str) :
            z_now = next((z for z in self.subzones if z.name == key), None)
            if not (z_now is None) :
                z_now.name = value
        else:
            raise ValueError("key must be a string or integer. key:", key)
    
    def __delitem__(self, key) :
        del self.subzones[key]
    
    def __contains__(self, key) :
        return key in self.subzones
    
    def __iter__(self) :
        return iter(self.subzones)
    
    # String Representation
    def __str__(self) -> str:
        return self.getResult().__str__()
    
    def __repr__(self) -> str:
        return self.__str__()

    # Operators
    def __mul__(self, other) :
        if isinstance(other, (int, float)) :
            return self.getResult() * other
        elif isinstance(other, BaseMulZone) :
            return self.getResult() * other.getResult()
        else :
            raise ValueError("Invalid type for multiplication: {}".format(type(other)))
    
    def __rmul__(self, other) :
        return self.__mul__(other)
    
    def __add__(self, other) :
        if isinstance(other, (int, float)) :
            return self.getResult() + other
        elif isinstance(other, BaseMulZone) :
            return self.getResult() + other.getResult()
        else :
            raise ValueError("Invalid type for addition: {}".format(type(other)))
    
    def __radd__(self, other) :
        return self.__add__(other)
    
    def __truediv__(self, other) :
        if isinstance(other, (int, float)) :
            return self.getResult() / other
        elif isinstance(other, BaseMulZone) :
            return self.getResult() / other.getResult()
        else :
            raise ValueError("Invalid type for division: {}".format(type(other)))
    
    def __rtruediv__(self, other) :
        if isinstance(other, (int, float)) :
            return other / self.getResult()
        elif isinstance(other, BaseMulZone) :
            return other.getResult() / self.getResult()
        else :
            raise ValueError("Invalid type for division: {}".format(type(other)))
    
    def __pow__(self, other) :
        if isinstance(other, (int, float)) :
            return self.getResult() ** other
        elif isinstance(other, BaseMulZone) :
            return self.getResult() ** other.getResult()
        else :
            raise ValueError("Invalid type for power: {}".format(type(other)))
    
    def __rpow__(self, other) :
        if isinstance(other, (int, float)) :
            return other ** self.getResult()
        elif isinstance(other, BaseMulZone) :
            return other.getResult() ** self.getResult()
        else :
            raise ValueError("Invalid type for power: {}".format(type(other)))

class MulZone(BaseMulZone):
    pass

class AddZone(BaseMulZone):
    def __init__(self, **kwargs):
        # 设置默认字典
        defaults = {
            "zoneType": "add",
            "val": 0.0,
            "result": 0.0
        }
        # 用传入的参数覆盖默认值
        defaults.update(kwargs)
        
        # 批量赋值到实例属性
        for key, value in defaults.items():
            setattr(self, key, value)

class CritZone(BaseMulZone):
    critRate = 0
    critDmg = 0.5
    def __init__(self, **kwargs) :
        defaults = {
            "zoneType": "mul",
            "val": 1.0,
            "result": 1.0,
            "critRate": 0.0,
            "critDmg": 0.5
        }
        defaults.update(kwargs)
        for key,value in defaults.items() :
            setattr(self, key ,value)
    
    def getResult(self) -> float :
        result = 1+self.critRate*self.critDmg
        return result