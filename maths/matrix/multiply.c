void multiplyRowMajor(int *A, int *B, int n, int *Dst) {
    int r = 400;

    for(i = 0; i < n/r; i++)
        for(j = 0; j < n/r; j++)
            for(k = 0; k < n/r; k++)
                Dst[j + i * n] += A[i * n + k]*B[k * n + j]
}

void multiplyColumnMajor(int *A, int *B, int n, int *Dst) {
    int r = 400;

    for(i = 0; i < n/r; i++)
        for(j = 0; j < n/r; j++)
            for(k = 0; k < n/r; k++)
                Dst[i + j * n] += A[k * n + i]*B[j * n + k]
}
