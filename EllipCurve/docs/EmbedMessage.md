# Embed Message in ECC

The main idea is find a point $M = Embed(m)$, and $m = Takeout(M)$.

## 1. `HashToX`

Designate a positive interger $k$, the exponent. Emumerate $m' \in \{ m' : m' = m + j, j \in \mathbb{Z}_k\}$. If $\exists M \in e, M_x = m'$, then $M = (m', y)$ is the embedding.

When taking out, $m = \lfloor \frac{M_x}{k} \rfloor$ is the plaintext.