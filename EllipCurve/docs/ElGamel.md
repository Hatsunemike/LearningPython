# `ElGamel` in ECC

## 1. Key Generation

1. Designate $e = E_p(a,b): \{ (x,y) : y^2 \equiv x^3 + ax + b ( \mod p), x,y \in \mathbb{Z}_p, y>0 \}$.
2. Designate or generate $G \in e$, $l = ord(G)$.
3. Randomly choose $x \in [2, \sqrt{l}] \cap \mathbb{Z}$, calculate $Y = xG$.
4. Designate $k$, the [HashToX](./EmbedMessage.md) exponent, usually $30$.
5. private key is $SK=(e, x, k)$, public key is $PK=(e, Y, G, \sqrt{l}, k)$.

## 2. pub-key Encryption

1. Embed plaintext $m$ in $e$: $M = HashToX(m, k)$.
2. Randomly choose $u \in [2, \sqrt{l}] \cap \mathbb{Z}$.
3. Calculate $C_1 = uG$, $C_2 = uY+M$.
4. Ciphertext is $C=(C_1, C_2)$.

## 3. pri-key Decryption

1. Calculate $M = C_2 - x C_1$.
2. Take out the plaintext $m = \lfloor \frac{M_x}{k} \rfloor$.