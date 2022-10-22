# test

$$
\begin{align*}
& s=s_1s_2\dots s_l,\ s_i \in \{0,1,?\} \\
& s^{(i)}=s_1s_2\dots s_i,\ s^{(0)}=\epsilon \\
& d=(d_1,d_2,\dots,d_k)
  =\sigma_1^{\{a_1,b_1\}}\sigma_2^{\{a_2,b_2\}}\dots\sigma_k^{\{a_k,b_k\}} \\
& d^{(j)}=(d_1,d_2,\dots,d_j),\ d^{(0)}=\epsilon \\
& A_j=\sum_{r=0}^{j} a_r,\ B_j=\sum_{r=0}^{j} b_r,\ a_0=b_0=0 \\
& \sigma\in\{0,1\},\ s_i\in\{0,1,?\},\ L_i^\sigma\left(s\right)=\max_{h \le i\ \wedge\ s_h \notin \{\sigma, ?\}}\left(h\right) \\
& Fix\left(s^{(0)},d^{(j)}\right)=true\text{ iff } A_j=0 \\
& \forall i,\ 1 \le i \le l,\ Fix\left(s^{(i)},d^{(0)}\right)=false \\
& Fix\left(s^{(i)},d^{(j)}\right)=
  \bigvee
  _{
    p=\max\left(
      i-b_j,
      A_{j-1},
      L_{i}^{\sigma_j}\left(s\right)
    \right)
  }
  ^{min(i-a_j, B_{j-1})}
  Fix\left(s^{(p)}, d^{(j-1)}\right),\ 1 \lt i \lt l,\ 1 \lt j \lt k, A_j \lt i \lt B_j \\
& d_7=0^{\{0,4\}}
    1^{\{2,2\}}
    0^{\{1,5\}}
    1^{\{1,1\}}
    0^{\{1,5\}}
    1^{\{1,1\}}
    0^{\{0,4\}} \\
& \mathbf{a}_7=\begin{pmatrix}0 & 2 & 1  & 1  & 1  & 1  & 0 \end{pmatrix}^T \\
& \mathbf{b}_7=\begin{pmatrix}4 & 2 & 5  & 1  & 5  & 1  & 4 \end{pmatrix}^T \\
& \mathbf{A}_7=\begin{pmatrix}0 & 2 & 3  & 4  & 5  & 6  & 6 \end{pmatrix}^T \\
& \mathbf{B}_7=\begin{pmatrix}4 & 6 & 11 & 12 & 17 & 18 & 22\end{pmatrix}^T \\
& s=s_1s_2\dots s_{10} \\
& Fix\left(5,6\right)=
  \bigvee
  _{
    p=\max\left(
      5, L_{5}^{'1'}\left(s\right)
    \right)
  }
  ^{4}
  Fix\left(p, 5\right) \\
& s=?11?0????0 \\
& Fix\left(10,7\right)=
  \bigvee
  _{
    p=\max\left(
      6,
      6,
      L_{10}^{\text{'0'}}\left(s\right)=3
    \right)=6
  }
  ^{10}
  Fix\left(p, 6\right) \\
\end{align*}
$$
