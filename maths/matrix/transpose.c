void transposeRowMajor( int n, int *dst, int *src ) {
    int i, j, x, y;
    int blocksize = 400;

    for(x = 0; x < n; x += blocksize)
        for(y = 0; y < n; y += blocksize)
            for(i = x; i < x + blocksize && i < n; i++)
                for(j = y; j < blocksize + y && j < n; j++)
                    dst[j + i*n] = src[i + j*n];
}

void transposeColumnMajor( int n, int *dst, int *src ) {
    int i, j, x, y;
    int blocksize = 400;

    for(x = 0; x < n; x += blocksize)
        for(y = 0; y < n; y += blocksize)
            for(i = x; i < x + blocksize && i < n; i++)
                for(j = y; j < blocksize + y && j < n; j++)
                    dst[i + j*n] src[j  + i*n];
}
