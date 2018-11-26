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
    # your code is here
    return Q, R

# INPUT : rectangular matrix A
# OUTPUT: matrices Q - orthogonal and R - upper triangular such that A = QR
def modified_gram_schmidt_qr(A): # 5 pts
    # your code is here
    return Q, R


# INPUT : rectangular matrix A
# OUTPUT: matrices Q - orthogonal and R - upper triangular such that A=QR
def householder_qr(A): # 7 pts
    # your code is here
    return Q, R


# INPUT:  G - np.ndarray
# OUTPUT: A - np.ndarray (of size G.shape)
def pagerank_matrix(G): # 5 pts
    if sp.sparse.issparse(G):
        G = sp.sparse.csr_matrix(G).astype(float)
    else:
        G = G.astype(float)
    G_sum = np.squeeze(np.asarray(G.sum(axis=0)))
    for col in range(G.shape[0]):
        if G_sum[col] == 0:
            #G[:,col] = np.repeat(1.0/G.shape[0], len(G_sum))
            pass
        else:
            G[:,col] = G[:,col] * 1.0/G_sum[col]
    # enter your code here
    return G


# INPUT:  A - np.ndarray (2D), x0 - np.ndarray (1D), num_iter -
# integer (positive)
# OUTPUT: x - np.ndarray (of size x0), l - float, res - np.ndarray (of size
# num_iter + 1 [include initial guess])
def power_method(A, x0, num_iter): # 5 pts
    rev = 0
    if (x0.shape[1] != 1):
        x0 = x0.T
        rev = 1
    x_k = x0.copy() / np.linalg.norm(x0, 2)
    res = np.zeros(num_iter + 1)
    l = np.dot(np.dot(A, x_k).T, x_k)
    res[0] = np.linalg.norm(A @ x_k - x_k * l, 2)
    for k in range(1, num_iter + 1):
        x_k = A @ x_k
        x_k = x_k / np.linalg.norm(x_k, 2)
        l = np.dot((A @ x_k).T, x_k)
        res[k] = np.linalg.norm(A @ x_k - x_k * l, 2)
    # enter your code here
    if (rev == 1):
        x_k = x_k.T
    return x_k, l, res


# INPUT:  A - np.ndarray (2D), d - float (from 0.0 to 1.0), x - np.ndarray
# (1D, size of A.shape[0/1])
# OUTPUT: y - np.ndarray (1D, size of x)
def pagerank_matvec(A, d, x): # 2 pts
    rev = 0
    if sp.sparse.issparse(A):
        A = sp.sparse.csr_matrix(A).astype(float)
    else:
        A = A.astype(float)
    sum_x = sum(x)
    val = (1-d)/A.shape[0]
    y = [d * np.dot(A[k,:], x) + val * sum_x for k in range(A.shape[0])]
    return y

def return_words():
    # insert the (word, cosine_similarity) tuples
    # for the words 'numerical', 'linear', 'algebra' words from the notebook
    # into the corresponding lists below
    # words_and_cossim = [('word1', 'cossim1'), ...]

    numerical_words_and_cossim = []
    linear_words_and_cossim = []
    algebra_words_and_cossim = []

    return numerical_words_and_cossim, linear_words_and_cossim, algebra_words_and_cossim
