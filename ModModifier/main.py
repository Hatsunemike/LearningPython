import re

filename = "merged.ini"

def processBody(match: re.Match) :
    indent1 = match.group(1)
    num = match.group(2)
    indent2 = match.group(3)
    return (f"{indent1}ps-t1 = ResourceArlecchinoBodyNormalMap.{num}\n"
            f"{indent2}ps-t2 = ResourceArlecchinoBodyLightMap.{num}\n"
    )

def processHead(match: re.Match) :
    num = match.group(2)
    return (f"\tps-t0 = ResourceArlecchinoHeadDiffuse.{num}\n"
	        f"\tps-t1 = ResourceArlecchinoHeadLightMap.{num}\n"
	        f"\tps-t2 = ResourceArlecchinoHeadNormalMap.{num}\n")

if __name__ == "__main__" :
    patternBody = re.compile(r"([ \t]*)ps-t0[ \t]*=[ \t]*ResourceArlecchinoBodyNormalMap\.(\d+)\n"
                             r"([ \t]*)ps-t2[ \t]*=[ \t]*ResourceArlecchinoBodyLightMap\.\d+\n",
                             re.MULTILINE)

    patternHead = re.compile(r"([ \t]*)ps-t0[ \t]*=[ \t]*ResourceArlecchinoHeadNormalMap\.(\d+)\n"
                             r"([ \t]*)ps-t1[ \t]*=[ \t]*ResourceArlecchinoHeadDiffuse\.(\d+)\n"
                             r"([ \t]*)ps-t2[ \t]*=[ \t]*ResourceArlecchinoHeadLightMap\.(\d+)\n")

    subConfigList = [(patternBody, processBody),
                     (patternHead, processHead)]

    with open(filename, encoding="utf-8", mode="r") as f:
        content = f.read()

    for (pattern, process) in subConfigList:    
        content = re.sub(pattern, process, content)

    with open("output.txt", encoding="utf-8", mode="w") as f:
        f.write(content)