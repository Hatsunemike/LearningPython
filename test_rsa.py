import rsa
import typing

def save_keys(
    publicKey: rsa.PublicKey,
    privateKey: rsa.PrivateKey
) -> None:
    with open("./public_key.pem", mode="wb") as public_f:
        with open("./private_key.pem", mode="wb") as private_f:
            public_f.write(publicKey.save_pkcs1())
            private_f.write(privateKey.save_pkcs1())

def load_keys() -> typing.Tuple[rsa.PublicKey, rsa.PrivateKey] :
    with open("./public_key.pem", mode="rb") as public_f:
        with open("./private_key.pem", mode="rb") as private_f:
            publicKey = rsa.PublicKey.load_pkcs1(public_f.read())
            privateKey = rsa.PrivateKey.load_pkcs1(private_f.read())
            return publicKey, privateKey
        
def print_keys(publicKey: rsa.PublicKey, privateKey: rsa.PrivateKey) -> None:
    print("============== RSA key values ============")
    print("p:", privateKey.p)
    print("q:", privateKey.q)
    print("n:", publicKey.n)
    print("e:", publicKey.e)
    print("d:", privateKey.d)
    print("==========================================")

publicKey, privateKey = rsa.newkeys(16, exponent=17)
publicKey, privateKey = load_keys()
save_keys(publicKey, privateKey)
print_keys(publicKey, privateKey)

message = 128
message_bytes = message.to_bytes(length=1, signed=False) # 转为无符号整数的字节流
# print("message_bytes: ", message_bytes)
encrypted_message = rsa.encrypt(message_bytes, pub_key=publicKey)

received_message_bytes = rsa.decrypt(encrypted_message,priv_key=privateKey)
received_message = int.from_bytes(received_message_bytes, signed = False)
print("received message: ", int(received_message))