'''
read.py: find the implicit file at the back of a .png file, and output it to a new file.
'''

import sys

BUF_SIZE = 8192

def strBytes(b: bytes) -> str :
    s = ""
    for x in b: s += chr(x)
    return s

def opthex(s: str) -> str:
    ret = ""
    for i in range(s.__len__()) :
        if(i%2 == 0) :
            now = i//2
            if now>0 and now%4==0 :
                ret += " "
            if now>0 and now%16==0 :
                ret += "\n"
        ret += s[i]
    return ret

filename = "./res.png"
output_name = "./re_output.zip"

def hexint(b: bytes, len=4) -> int: # big-endian
    r = 0
    for i in b[:len] :
        r = r*256+i
    return r

with open(filename, mode="rb") as f:
    png_sig = f.read(8) # 'sig' field
    if png_sig != bytes([0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A]) :
        print("the file isn't png:", filename, file=sys.stderr)
        exit()
    
    readed = 0
    cnt = 1
    while 1: # read chunk
        length = hexint(f.read(4))
        readed += 4
        b_type = f.read(4).decode()
        readed += 4
        _ = f.read(length) # filt the data
        readed += length
        crc = f.read(4)
        readed += 4
        if b_type == "IEND":
            break
        # print("length:", length, "type:", b_type, "crc:", crc, "readed:", readed)
        cnt += 1
    
    of = open(output_name, mode="wb")
    total_size = 0
    while 1:
        buf = f.read(BUF_SIZE)
        if len(buf) == 0:break
        of.write(buf)
        total_size += len(buf)
    print("total write:", total_size)
    of.close()