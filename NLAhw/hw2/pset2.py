# encoding: utf-8
# pset2.py

import numpy as np
import scipy as sp
# don't forget import packages, e.g. scipy
# but make sure you didn't put unnecessary stuff in here

# INPUT : diag_broadcast - list of diagonals value to broadcast,length equal
# to 3 or 5; n - integer, band matrix shape.
# OUTPUT : L - 2D np.ndarray, L.shape[0] depends on bandwidth,
# L.shape[1] = n-1, do not store main diagonal, where all ones;
# add zeros to the right side of rows to handle with changing length of diagonals.
#          U - 2D np.ndarray, U.shape[0] = n, U.shape[1] depends on bandwidth;
#              add zeros to the bottom of columns to handle with changing length of diagonals.
def mband3lu(db, n):
    L, U = np.zeros((1, n - 1)), np.zeros((n, 2))
    U[0, 0], U[0, 1] = db[1], db[2]
    U[0, 1] = db[2]
    for k in range(1, n):
        U[k - 1, 1] = db[2]
        L[0, k - 1] = db[0] / U[k - 1, 0]
        U[k, 0] = db[1] - db[2] * db[0] / U[k - 1, 0]
    return L, U

def mband5lu(db, n):
    L, U = np.zeros((2, n - 1)), np.zeros((n, 3))
    U[0, 0], U[0, 1], U[0, 2] = db[2], db[3], db[4]
    U[1, 0] = db[2] - U[0, 1] * L[0, 0]
    L[0, 0] = db[1] / db[2]
    for k in range(n - 2):
        L[1, k] = db[0] / U[k, 0]
        L[0, k + 1] = (db[1] - U[k, 1] * L[1, k]) / U[k + 1, 0]
        U[k + 1, 1] = db[3] - L[0, k] * db[4]
        U[k + 2, 0] = db[2] - L[0, k + 1] * U[k + 1, 1] - L[1, k] * db[4]
        U[k, 2] = db[4]
    return L, U

def band_lu(diag_broadcast, n): # 5 pts
    diag_len = len(diag_broadcast)
    if (diag_len == 3):
        L, U = mband3lu(diag_broadcast, n)
    elif (diag_len == 5):
        L, U = mband5lu(diag_broadcast, n)
    else:
        L = np.zeros((diag_len // 2, n - 1))
        U = np.zeros((n, diag_len // 2 + 1))
    return L, U

# INPUT : rectangular matrix A
# OUTPUT: matrices Q - orthogonal and R - upper triangular such that A = QR
def gram_schmidt_qr(A): # 5 pts
    Q = np.zeros_like(A)
    R = np.zeros((A.shape[1], A.shape[1]))
    R[0,0], Q[:,0] = 1, A[:,0]

    for k in range(1, A.shape[1]):
        v, r = A[:, k], R[:, k]
        r[k] = 1
        r[:k] = [(np.dot(v, Q[:, i])) / (np.dot(Q[:, i], Q[:, i])) for i in range(k)]
        Q[:, k] = v - np.sum(r[:k] * Q[:,:k], axis=1)
    #Normalization
    for k in range(A.shape[1]):
        norm = np.linalg.norm(Q[:,k], 2)
        Q[:,k], R[k,:] = Q[:,k] / norm, R[k,:] * norm
    return Q, R

# INPUT : rectangular matrix A
# OUTPUT: matrices Q - orthogonal and R - upper triangular such that A = QR
def modified_gram_schmidt_qr(A): # 5 pts
    n = A.shape[1] + 1
    Q = np.zeros((n, n))
    R, V = Q.copy(), Q.copy()
    V[1:,1:] = A

    for k in range(1, n):
        R[k][k] = np.linalg.norm(V[:,k], 2)
        Q[:,k] = V[:,k] / R[k][k]
        for i in range(k + 1, n):
            R[k][i] = np.dot(Q[:,k].T, V[:,i])
            V[:,i] -= R[k][i] * Q[:,k]

    return Q[1:, 1:], R[1:, 1:]

# INPUT : rectangular matrix A
# OUTPUT: matrices Q - orthogonal and R - upper triangular such that A=QR
def householder_qr(A): # 7 pts
    m, n = A.shape
    e, Q, R = np.zeros(A.shape[0]), np.identity(A.shape[0]), A
    e[0] = 1

    for k in range(n):
        x = R[k:, k]
        x_norm = np.linalg.norm(x, 2)
        if (x[0] >= 0):
            v = x + x[0] * x_norm * e[:m-k]
        else:
            v = x - x[0] * x_norm * e[:m-k]
        v /= np.linalg.norm(v, 2)
        H = np.identity(m)
        H[k:,k:] = np.identity(m-k) - 2 * np.outer(v, v)
        Q, R = np.dot(H,Q), np.dot(H, R)

    return Q.T, R

# INPUT:  G - np.ndarray
# OUTPUT: A - np.ndarray (of size G.shape)
def pagerank_matrix(G): # 5 pts
    sparse = 0
    if sp.sparse.issparse(G):
        G = G.astype(np.float64)
        diags = np.squeeze(np.asarray(np.sum(G, axis=1)))
        diags = [1.0/x if x != 0 else 1 for x in diags]
        D = sp.sparse.diags(diags, 0, format='lil', dtype=np.float64)
        return G.T.dot(D)
    else:
        A = G.astype(float)

    for col in range(A.shape[0]):
        nzi = G[:,col].nonzero()[0]
        row_sum = np.sum(G[nzi, col])
        if row_sum == 0:
            A[:,col] = 1.0/G.shape[0]
        else:
            #A[G[:,col].nonzero()[0],col] = 1.0/row_sum
            A[nzi, col] = 1.0/row_sum
    return A


# INPUT:  A - np.ndarray (2D), x0 - np.ndarray (1D), num_iter -
# integer (positive)
# OUTPUT: x - np.ndarray (of size x0), l - float, res - np.ndarray (of size
# num_iter + 1 [include initial guess])
def power_method(A, x0, num_iter): # 5 pts
    rev = 0
    if (x0.shape[1] != 1):
        x0 = x0.T
        rev = 1
    x_k = x0 / np.linalg.norm(x0, 2)
    res = np.zeros(num_iter + 1)
    l = A.dot(x_k).T.dot(x_k)
    res[0] = np.linalg.norm(A.dot(x_k) - x_k * l, 2)
    for k in range(1, num_iter + 1):
        x_k = A.dot(x_k)
        x_k = x_k / np.linalg.norm(x_k, 2)
        Axk = A.dot(x_k)
        l = Axk.T.dot(x_k)
        res[k] = np.linalg.norm(Axk - x_k * l, 2)
    if (rev == 1):
        x_k = x_k.T
    return x_k, l, res


# INPUT:  A - np.ndarray (2D), d - float (from 0.0 to 1.0), x - np.ndarray
# (1D, size of A.shape[0/1])
# OUTPUT: y - np.ndarray (1D, size of x)
def pagerank_matvec(A, d, x): # 2 pts
    rev = 0
    A = A.astype(float)
    y = d * A.dot(x) + (1-d)/A.shape[0] * np.sum(x)
    return y

def return_words():
    # insert the (word, cosnp.linalg.norm(x, 2) *np.linalg.norm(x, 2) *ine_similarity) tuples
    # for the words 'numerical', 'linear', 'algebra' words from the notebook
    # into the corresponding lists below
    # words_and_cossim = [('word1', 'cossim1'), ...]

    numerical_words_and_cossim = []
    linear_words_and_cossim = []
    algebra_words_and_cossim = []

    return numerical_words_and_cossim, linear_words_and_cossim, algebra_words_and_cossim
