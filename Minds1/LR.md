#LR factorization
---

```
A =

     2     1     1
     1     3     2
     1     2     2
```
factorization
```
L = [2,1,1; 0,2.5,1.5; 0,0,0.6]

L =

    2.0000    1.0000    1.0000
         0    2.5000    1.5000
         0         0    0.6000

R = [1,0,0; 0.5,1,0; 0.5,0.6,1.0]

R =

    1.0000         0         0
    0.5000    1.0000         0
    0.5000    0.6000    1.0000

L*R = A
```

###Matlab
---
The lu function expresses a matrix A as the product of two essentially triangular matrices, one of them a permutation of a lower triangular matrix and the other an upper triangular matrix. The factorization is often called the LU, or sometimes the LR, factorization. A can be rectangular.

```
[L,U,P,Q,R] = lu(A)
```

