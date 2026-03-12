#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifndef CLK_TCK
#define CLK_TCK CLOCKS_PER_SEC
#endif



int *read_all(const char *name, int *n)
{
    FILE *f = fopen(name, "r");
    if (!f)
        return NULL;

    int cap = 1024;
    *n = 0;
    int *a = malloc(cap * sizeof(int));

    while (fscanf(f, "%d", &a[*n]) == 1)
    {
        (*n)++;
        if (*n >= cap)
        {
            cap *= 2;
            a = realloc(a, cap * sizeof(int));
        }
    }
    fclose(f);
    return a;
}

void copy(int *d, int *s, int n)
{
    memcpy(d, s, n * sizeof(int));
}

/*  Bubble Sort  */

void bubbleSort(int a[], int n)
{
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (a[j] > a[j + 1])
            {
                int t = a[j];
                a[j] = a[j + 1];
                a[j + 1] = t;
            }
}

/*  Quick Sort  */

int partition(int a[], int l, int h)
{
    int p = a[h], i = l - 1;
    for (int j = l; j < h; j++)
        if (a[j] <= p)
        {
            i++;
            int t = a[i];
            a[i] = a[j];
            a[j] = t;
        }
    int t = a[i + 1];
    a[i + 1] = a[h];
    a[h] = t;
    return i + 1;
}

void quickSortRec(int a[], int l, int h)
{
    if (l < h)
    {
        int p = partition(a, l, h);
        quickSortRec(a, l, p - 1);
        quickSortRec(a, p + 1, h);
    }
}

void quickSort(int a[], int n)
{
    quickSortRec(a, 0, n - 1);
}

/*  Merge Sort  */

void merge(int a[], int t[], int l, int m, int r)
{
    int i = l, j = m + 1, k = l;
    while (i <= m && j <= r)
        t[k++] = (a[i] <= a[j]) ? a[i++] : a[j++];
    while (i <= m)
        t[k++] = a[i++];
    while (j <= r)
        t[k++] = a[j++];
    for (i = l; i <= r; i++)
        a[i] = t[i];
}

void mergeRec(int a[], int t[], int l, int r)
{
    if (l < r)
    {
        int m = (l + r) / 2;
        mergeRec(a, t, l, m);
        mergeRec(a, t, m + 1, r);
        merge(a, t, l, m, r);
    }
}

void mergeSort(int a[], int n)
{
    int *t = malloc(n * sizeof(int));
    mergeRec(a, t, 0, n - 1);
    free(t);
}

/*  Binary Insertion Sort  */

int binSearch(int a[], int x, int l, int r)
{
    while (l <= r)
    {
        int m = (l + r) / 2;
        if (a[m] <= x)
            l = m + 1;
        else
            r = m - 1;
    }
    return l;
}

void binaryInsertionSort(int a[], int n)
{
    for (int i = 1; i < n; i++)
    {
        int key = a[i];
        int pos = binSearch(a, key, 0, i - 1);
        for (int j = i - 1; j >= pos; j--)
            a[j + 1] = a[j];
        a[pos] = key;
    }
}

/*  Timing  */

double timeSort(void (*sort)(int *, int), int *src, int *tmp, int n)
{
    int K = 1;
    clock_t ticks;
    do
    {
        clock_t s = clock();
        for (int i = 0; i < K; i++)
        {
            copy(tmp, src, n);
            sort(tmp, n);
        }
        ticks = clock() - s;
        K *= 2;
    } while (ticks < 10);

    return ((double)ticks / CLK_TCK) / (K / 2);
}



int main()
{
    int n;
    int *arr = read_all("input.txt", &n);
    int *tmp = malloc(n * sizeof(int));

    FILE *out = fopen("output.txt", "w");
    double t_bubble = timeSort(bubbleSort, arr, tmp, n);
    double t_quick = timeSort(quickSort, arr, tmp, n);
    double t_merge = timeSort(mergeSort, arr, tmp, n);
    double t_bin = timeSort(binaryInsertionSort, arr, tmp, n);
    /* ---- Print to terminal ---- */
    printf("\n==== Sorting Performance (Terminal) ====\n");
    printf("Bubble Sort Time: %f seconds\n", t_bubble);
    printf("Quick Sort Time: %f seconds\n", t_quick);
    printf("Merge Sort Time: %f seconds\n", t_merge);
    printf("Binary Insertion Sort Time: %f seconds\n", t_bin);

    fprintf(out, "Total numbers: %d\n\n", n);

    // Bubble
    fprintf(out, "=== Bubble Sort ===\n");
    fprintf(out, "Time: %f sec\n", timeSort(bubbleSort, arr, tmp, n));
    copy(tmp, arr, n);
    bubbleSort(tmp, n);
    for (int i = 0; i < n; i++)
        fprintf(out, "%d\n", tmp[i]);

    // Quick
    fprintf(out, "\n=== Quick Sort ===\n");
    fprintf(out, "Time: %f sec\n", timeSort(quickSort, arr, tmp, n));
    copy(tmp, arr, n);
    quickSort(tmp, n);
    for (int i = 0; i < n; i++)
        fprintf(out, "%d\n", tmp[i]);

    // Merge
    fprintf(out, "\n=== Merge Sort ===\n");
    fprintf(out, "Time: %f sec\n", timeSort(mergeSort, arr, tmp, n));
    copy(tmp, arr, n);
    mergeSort(tmp, n);
    for (int i = 0; i < n; i++)
        fprintf(out, "%d\n", tmp[i]);

    // Binary Insertion
    fprintf(out, "\n=== Binary Insertion Sort ===\n");
    fprintf(out, "Time: %f sec\n", timeSort(binaryInsertionSort, arr, tmp, n));
    copy(tmp, arr, n);
    binaryInsertionSort(tmp, n);
    for (int i = 0; i < n; i++)
        fprintf(out, "%d\n", tmp[i]);

    fclose(out);
    free(tmp);
    free(arr);
    return 0;
}
