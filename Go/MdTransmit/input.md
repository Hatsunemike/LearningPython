# 0. 问题模型与基本记号

给定循环群 $(G,\cdot)$，生成元 $g\in G$，目标元素 (h\in G)。离散对数问题是求
[
x=\log_g h\quad\text{使得}\quad g^x=h.
]
常见群：有限域乘法群 (\mathbb{F}*p^\times)、(\mathbb{F}*{p^k}^\times)，以及椭圆曲线群 (E(\mathbb{F}_q))（用加法记号，等式写作 (xP=Q)）。

**通用下界（Shoup）**：在“泛群模型”中，任意**泛型**算法求 DLP 需要期望 (\Omega(\sqrt{n})) 次群运算（(n=\lvert G\rvert) 的大素因子规模）。
这解释了为何很多“泛用”算法时间复杂度是 (\tilde O(\sqrt{n}))。

---

# 1. Shanks 的婴儿步–巨人步（BSGS）

**思想**：把指数写成 (x=j m+i)，令 (m=\lceil\sqrt{n}\rceil)。

**步骤与公式**
[
\begin{aligned}
&m=\lceil \sqrt{n}\rceil,\qquad c=g^{-m}.\
&\text{（婴儿步）预存 } g^i,\ i=0,1,\dots,m-1.\
&\text{（巨人步）对 } j=0,1,\dots,m \text{ 计算 } h\cdot c^{,j},\ \text{如命中某个 } g^i,\
&\text{则 } x=j m+i.
\end{aligned}
]

**复杂度**：时间 (O(\sqrt{n}))，空间 (O(\sqrt{n}))。

**应用场景**：中等规模群的单次求解（例如教学/CTF/竞赛），或当可以容忍 (\Theta(\sqrt{n})) 级别内存时。
**例**：在 (\mathbb{F}_{101}^\times) 中求 (\log_2 37) 时，取 (m=10)，按上式建表并迭代碰撞即可。

---

# 2. Pollard’s Rho（离散对数版）

**思想**：随机游走找碰撞，但始终维持走到的点可表成 (X_i=g^{a_i}h^{b_i})。

**典型分区更新**：把群划分成 (S_1,S_2,S_3)，若 (X_i\in S_1) 则 (X_{i+1}=gX_i)（((a,b)\leftarrow(a+1,b))）；若在 (S_2) 则 (X_{i+1}=hX_i)（((a,b)\leftarrow(a,b+1))）；若在 (S_3) 则 (X_{i+1}=X_i^2)（((a,b)\leftarrow(2a,2b))）等。

**碰撞解方程**：若出现 (X_i=X_j\ (i\neq j))，则
[
g^{a_i}h^{b_i}=g^{a_j}h^{b_j}\ \Longrightarrow\ g^{a_i-a_j}=h^{b_j-b_i}=g^{x(b_j-b_i)},
]
从而
[
x\equiv (a_i-a_j),(b_j-b_i)^{-1}\pmod n.
]

**复杂度**：时间期望 (O(\sqrt{n}))，空间 (O(1))。易并行（van Oorschot–Wiener 的“显著点”技术）。

**应用场景**：内存紧张的大素数阶群，尤其是椭圆曲线 DLP（ECDLP）。
**注**：在椭圆曲线群可用“取负映射”加速，期望常数因子可降到 (\approx \sqrt{\pi n/2})。

---

# 3. Pollard’s Kangaroo（(\lambda) 算法）

**思想**：当已知 (x\in [L,U]) 的区间时，用“家养袋鼠”（从 (U) 左右出发）与“野生袋鼠”（从 (h) 出发）做受控跳跃，期望在 ([L,U]) 长度的平方根量级内相遇。

**复杂度**：时间 (O\big(\sqrt{U-L}\big))，空间 (O(1))。

**应用场景**：已知指数有范围约束（如某些协议把随机指数限制在区间内；或密码取证中已掌握粗略范围）。

---

# 4. Pohlig–Hellman（群阶可分解）

若 (\lvert G\rvert = n=\prod_{i=1}^t \ell_i^{e_i})，则可把 DLP 分解为模各个 (\ell_i^{e_i}) 的子问题并用中国剩余定理（CRT）合成。

**核心公式**：对每个 (\ell=\ell_i) 设
[
g_\ell = g^{n/\ell^{e}},\qquad h_\ell = h^{n/\ell^{e}},
]
则
[
x_\ell = \log_{g_\ell} h_\ell \pmod{\ell^{e}}.
]
把 (x_\ell) 们用 CRT 还原为 (x\bmod n)。

**逐位提升（(\ell^e)）**：写
[
x = x_0 + x_1\ell + \cdots + x_{e-1}\ell^{e-1},
]
先求
[
h^{n/\ell}\equiv g^{(n/\ell)x_0},
]
得 (x_0)；再令
[
c_1 = h,g^{-x_0},\quad c_1^{n/\ell^2}\equiv g^{(n/\ell^2)x_1},
]
类推求 (x_1,x_2,\dots)。

**复杂度**：由最大素因子 (\max \ell_i) 主导；每个素因子层面通常用 BSGS/ρ 求解 (\tilde O(\sqrt{\ell_i}))，总体 (\tilde O!\big(\sum e_i\sqrt{\ell_i}\big))。

**应用场景**：当群阶 (n) 有小素因子或是“光滑数”（smooth）时极其有效。
**安全启示**：密码学里常选**素数阶**或有巨大素因子的群来抵抗该法（例如选 (p=2q+1) 的“安全素数”，在 (\mathbb{F}_p^\times) 里把问题落在阶为 (q) 的子群）。

---

# 5. 指数计算/指数演算法（Index Calculus）与 NFS-DL/FFS

这类是**非泛型**、利用表示结构的“亚指数级”算法，适用于有限域乘法群（非椭圆曲线）。

## 5.1 经典 Index Calculus（大特征有限域）

**基本流程**

1. 选取因子基 (\mathcal{B}={\text{小素数或小不可约多项式}\le B})。
2. 随机取 (e)，若
   [
   g^e \bmod p=\prod_{\ell\in\mathcal{B}} \ell^{a_{\ell}},
   ]
   即“(B)-光滑”，则得到**关系**：
   [
   e \equiv \sum_{\ell\in\mathcal{B}} a_{\ell},\log_g \ell \pmod{p-1}.
   ]
   重复收集，解线性方程求出 (\log_g \ell)。
3. 对目标 (h)，找 (e) 使 (h\cdot g^e) 也 (B)-光滑，得
   [
   \log_g h \equiv \sum a_{\ell},\log_g \ell - e \pmod{p-1}.
   ]

**复杂度**：大致 (L_p[1/2,c]) 型；在素数域上远慢于 NFS-DL（见下）。

## 5.2 数域筛离散对数（NFS-DL，适用于 (\mathbb{F}_p^\times)）

**复杂度（标准记号）**
[
L_p!\left[\tfrac{1}{3},\ \left(\tfrac{64}{9}\right)^{!1/3}\right]
=\exp!\left(\big((64/9)^{1/3}+o(1)\big)(\log p)^{1/3}(\log\log p)^{2/3}\right),
]
是目前大特征素数域 DLP 的最佳已知经典算法。

**应用场景**：破解 (\mathbb{F}_p^\times) 上的 DH/ElGamal（足够大的参数仍安全，但等强度需要比 ECC 更长的位数）。

## 5.3 函数域筛（FFS）与小特征域算法

在小特征 (\mathbb{F}_{p^k})（尤其 (p) 很小、(k) 适中）可用 FFS 或更快的变体（例如 Joux 2013 的“准多项式级”思路），对某些参数族非常高效。
**安全启示**：小特征场上的 DLP 更应谨慎选参。

---

# 6. 椭圆曲线上的特殊降维/特殊曲线攻击

## 6.1 MOV（Menezes–Okamoto–Vanstone）与 Frey–Rück 降维

利用对偶配对（Weil/Tate pairing）把 ECDLP 映到有限域乘法群：
若 (n\mid (q^k-1))（(k) 为嵌入度），可取 (P,Q\in E[n]) 与某固定 (R)，则
[
e_k(P,R)\in \mu_n\subset \mathbb{F}*{q^k}^\times,\qquad
\frac{\log, e_k(Q,R)}{\log, e_k(P,R)} \equiv \log_P Q \pmod n.
]
**应用场景**：嵌入度 (k) 很小（如超奇异曲线）时，ECDLP 可被规约到 (\mathbb{F}*{q^k}^\times) 上，用 NFS-DL/FFS 处理。
**安全启示**：实际选曲线时需避免小嵌入度（使用“配对友好曲线”时也要把 (q^k) 变得足够大）。

## 6.2 “异常曲线”攻击（Smart / Semaev–Smart–Satoh–Araki）

若 (\lvert E(\mathbb{F}_p)\rvert=p)（异常曲线），可通过形式群对数把 ECDLP 规约到 (\mathbb{Z}/p\mathbb{Z}) 的线性问题，从而**不安全**。
**安全启示**：生成曲线时排除异常曲线与其他已知弱曲线族。

---

# 7. 其他提速与结构化算法

* **Cheon 算法（带辅助输入）**：若已知 (n) 的某约数 (d) 并可取到 (g^{n/d},h^{n/d}) 等辅助，时间可达
  [
  \tilde O!\big(\sqrt{n/d}+\sqrt{d}\big),
  ]
  适用于存在额外群结构或端同态的场景（某些配对友好曲线族要避雷）。
* **并行化与实现技巧**：ρ 的“显著点”、多核/分布式；ECC 上的“取负映射”“自动同态”分区（Teske 变种）等，显著改善常数因子。

---

# 8. 量子算法（对称信息）

* **Shor 算法**：在量子计算模型下，DLP 可在
  [
  \operatorname{poly}(\log n)
  ]
  时间内求解。这是为何量子时代传统 DH / ECDH / ECDSA 会失守、需要后量子方案的根本原因。
  （在可预见的现实硬件规模到来前，经典参数依旧按传统强度评估。）

---

# 9. 选参与“哪种算法最相关”的速配表

* (\mathbb{F}_p^\times)（素数域，大特征）：
  (\rightarrow) NFS-DL（实际最强），PH（若 (p-1) 不“粗糙”），BSGS/ρ（通用基线）。
* (\mathbb{F}_{p^k}^\times)（小特征或特殊扩域）：
  (\rightarrow) FFS/小特征算法；谨慎避开被已知法高效处理的参数族。
* (E(\mathbb{F}_q))（椭圆曲线）：
  (\rightarrow) 泛型下界生效，主要是 Pollard ρ / 袋鼠；避开异常曲线与小嵌入度（否则 MOV/Frey–Rück 降维 + NFS/FFS）。
* 已知指数范围 ([L,U])：
  (\rightarrow) 袋鼠法 (O!\big(\sqrt{U-L}\big))。
* 群阶可分解（有小素因子）：
  (\rightarrow) Pohlig–Hellman + CRT。
* 有辅助输入/端同态结构：
  (\rightarrow) Cheon 算法等结构化加速。

---

# 10. 一个小而完整的“公式化”示例

设 (\lvert G\rvert=n=\ell_1^{e_1}\ell_2^{e_2}) 且 (\ell_1\gg \ell_2)。要解 (\log_g h)：

1. **Pohlig–Hellman 分解**
   [
   x_{\ell_i}=\log_{g^{n/\ell_i^{e_i}}}!!\big(h^{,n/\ell_i^{e_i}}\big)\pmod{\ell_i^{e_i}},\quad i=1,2.
   ]
2. **子问题求解**
   对大的 (\ell_1) 用 Pollard ρ（时间 (\tilde O(\sqrt{\ell_1}))、常数内存）；对小的 (\ell_2) 用 BSGS（快且内存可接受）。
3. **CRT 合并**
   [
   x\equiv x_{\ell_1}\pmod{\ell_1^{e_1}},\qquad
   x\equiv x_{\ell_2}\pmod{\ell_2^{e_2}}
   \ \Longrightarrow\ x\bmod n.
   ]
