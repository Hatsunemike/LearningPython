import hashlib
import sys

filename = sys.argv[1] if len(sys.argv) > 1 else 'hashVerify.py'
trueHash = sys.argv[2] if len(sys.argv) > 2 else ""

with open(filename, mode='rb') as f:
    batch_size = 8192
    m = hashlib.sha256()
    chunk = f.read(batch_size)
    while len(chunk) > 0:
        m.update(chunk)
        chunk = f.read(batch_size)
    result = m.hexdigest()
    print("result:", result)
    if result.upper() == trueHash:
        print("OK")
    else:
        print("Fail")