#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#define ALGINSERTION 1
#define ALGSELECTION 2
#define ALGQSORT 3
#define ALGQSORT3 4
#define ALGQSORTINS 5
#define ALGQSORT3INS 6
#define ALGSHELLSORT 7
#define ALGRECSEL 8
#define ALGMERGE 9
#define ALGBUBBLE 10
#define ALGRDX 11
#define ALGBKT 12
#define ALGCOUNT 13

typedef struct alg
{
    int num;
    char *name;
} alg_t;

alg_t algvet[] = {
    {ALGINSERTION, "i"},
    {ALGSELECTION, "s"},
    {ALGQSORT, "q"},
    {ALGQSORT3, "q3"},
    {ALGQSORTINS, "qi"},
    {ALGQSORT3INS, "q3i"},
    {ALGSHELLSORT, "h"},
    {ALGRECSEL, "rs"},
    {ALGMERGE, "ms"},
    {ALGBUBBLE, "bs"},
    {ALGRDX, "rd"},
    {ALGBKT, "bk"},
    {ALGCOUNT, "cs"},
    {0, 0}};

int name2num(char *name)
{
    int i = 0;
    while (algvet[i].num)
    {
        if (!strcmp(algvet[i].name, name))
            return algvet[i].num;
        i++;
    }
    return 0;
}

char *num2name(int num)
{
    int i = 0;
    while (algvet[i].num)
    {
        if (algvet[i].num == num)
            return algvet[i].name;
        i++;
    }
    return 0;
}

typedef struct opt
{
    int size;
    int seed;
    int alg;
} opt_t;

typedef struct sortperf
{
    int cmp;
    int move;
    int calls;
} sortperf_t;

void resetcounter(sortperf_t *s)
{
    // Descricao: inicializa estrutura
    // Entrada:
    // Saida: s
    s->cmp = 0;
    s->move = 0;
    s->calls = 0;
}

void inccmp(sortperf_t *s, int num)
{
    // Descricao: incrementa o numero de comparacoes em num
    // Entrada: s, num
    // Saida: s
    s->cmp += num;
}

void incmove(sortperf_t *s, int num)
{
    // Descricao: incrementa o numero de movimentacoes de dados em num
    // Entrada: s, num
    // Saida: s
    s->move += num;
}

void inccalls(sortperf_t *s, int num)
{
    // Descricao: incrementa o numero de chamadas de função em num
    // Entrada: s, num
    // Saida: s
    s->calls += num;
}

char *printsortperf(sortperf_t *s, char *str, char *pref)
{
    // Descricao: gera string com valores de sortperf
    // Entrada: s, pref
    // Saida: str

    sprintf(str, "%s cmp %d move %d calls %d", pref, s->cmp, s->move, s->calls);
    return str;
}

void initVector(int *vet, int size)
{
    // Descricao: inicializa vet com valores aleatorios
    // Entrada: vet
    // Saida: vet
    int i;
    // inicializa a parte alocada da vetor com valores aleatorios
    for (i = 0; i < size; i++)
    {
        vet[i] = size - i;
    }
}

void printVector(int *vet, int size)
{
    // Descricao: inicializa vet com valores aleatorios
    // Entrada: vet
    // Saida: vet
    int i;
    for (i = 0; i < size; i++)
    {
        printf("%d ", vet[i]);
    }
    printf("\n");
}

void swap(int *xp, int *yp, sortperf_t *s)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

// shellsort
void shellSort(int *A, int n, sortperf_t *s)
{
    for (int h = n / 2; h > 0; h /= 2)
    {
        for (int i = h; i < n; i += 1)
        {
            int temp = A[i];
            int j;
            for (j = i; j >= h && A[j - h] > temp; j -= h)
            {
                A[j] = A[j - h];
            }
            A[j] = temp;
        }
    }
}

// selection sort

void selectionSort(int arr[], int l, int r, sortperf_t *s)
{
    int i, j, Min;
    for (i = l; i < r; i++)
    {
        Min = i;
        for (j = i + 1; j <= r; j++)
        {
            if (arr[j] < arr[Min])
                Min = j;
        }
        if (Min != i)
        {
            swap(&arr[i], &arr[Min], s);
        }
    }
}

// insertion sort
void insertionSort(int v[], int l, int r, sortperf_t *s)
{
    int i, j;
    int aux;

    for (i = l + 1; i <= r; i++)
    {
        aux = v[i];
        j = i - 1;

        while (j >= l && aux < v[j])
        {
            v[j + 1] = v[j];
            j--;
        }

        v[j + 1] = aux;
    }
}

// median of 3 integers
int median(int a, int b, int c)
{
    if ((a <= b) && (b <= c))
        return b; // a b c
    if ((a <= c) && (c <= b))
        return c; // a c b
    if ((b <= a) && (a <= c))
        return a; // b a c
    if ((b <= c) && (c <= a))
        return c; // b c a
    if ((c <= a) && (a <= b))
        return a; // c a b
    return b;     // c b a
}

// standard quicksort partition
void partition(int *A, int l, int r, int *i, int *j, sortperf_t *s)
{
    int x;
    *i = l;
    *j = r;
    x = A[(*i + *j) / 2];
    do
    {
        while (x > A[*i])
        {
            (*i)++;
        }
        while (x < A[*j])
        {
            (*j)--;
        }
        if (*i <= *j)
        {
            swap(&A[*i], &A[*j], s);
            (*i)++;
            (*j)--;
        }
    } while (*i <= *j);
}

// MergeSort

void merge(int *A, int l, int m, int r, sortperf_t *s)
{
    int n1 = m - l + 1;
    int n2 = r - m;

    int *L = (int *)malloc(n1 * sizeof(int));
    int *R = (int *)malloc(n2 * sizeof(int));

    for (int i = 0; i < n1; i++)
        L[i] = A[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = A[m + 1 + j];

    int i = 0;
    int j = 0;
    int k = l;

    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            A[k] = L[i];
            i++;
        }
        else
        {
            A[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1)
    {
        A[k] = L[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        A[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(int *A, int l, int r, sortperf_t *s)
{
    if (l < r)
    {
        int meio = l + (r - l) / 2;
        mergeSort(A, l, meio, s);
        mergeSort(A, meio + 1, r, s);
        merge(A, l, meio, r, s);
    }
}

void quickSort(int *A, int l, int r, sortperf_t *s)
{
    int i = l;
    int j = r;
    partition(A, l, r, &i, &j, s);
    if (l < j)
    {
        quickSort(A, l, j, s);
    }
    if (i < r)
    {
        quickSort(A, i, r, s);
    }
}

void bubbleSort(int arr[], int n, sortperf_t *s)
{
    int i, j;

    for (i = 0; i < n - 1; i++)
    {
        for (j = 1; j < n - i; j++)
        {
            if (arr[j] < arr[j - 1])
            {
                swap(&arr[j], &arr[j - 1], s);
            }
        }
    }
}

// radix sort
int digit(int num, int k) {
    return (num >> k) & 1;
}

void exch(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}
void quicksortB(int v[], int l, int r, int w, sortperf_t *s)
{
    int i = l, j = r;
    if (r <= l || w > 0)
        return;
    while (j != i)
    {
        while (digit(v[i], w) == 0 && (i < j))
            i++;
        while (digit(v[j], w) == 1 && (j > i))
            j--;
        exch(v[i], v[j]);
    }
    if (digit(v[r], w) == 0)
        j++;
    quicksortB(v, l, j - 1, w - 1, s);
    quicksortB(v, j, r, w - 1, s);
}
void radixSort(int v[], int n, sortperf_t *s) {
    int maxBits = sizeof(int) * 8;
    quicksortB(v, 0, n - 1, maxBits-1,s);
}

void countingSort(int *A, int n, int max, sortperf_t *s)
{
    int *counts = (int *)calloc(max, sizeof(int));
    int i, j;
    for (i = 0; i < n; i++)
    {
        counts[A[i]]++;
    }
    i = 0;
    for (j = 0; j < max; j++)
    {
        while (counts[j] > 0)
        {
            A[i++] = j;
            counts[j]--;
        }
    }
}

int getMaxMin(int *A, int n)
{
    int maximo = A[0];
    for (int i = 1; i < n; i++)
    {
        if (A[i] > maximo)
        {
            maximo = A[i];
        }
    }
    return maximo;
}

void bucketSort(int *A, int n, sortperf_t *s)
{
    int bucket[n + 1];

    const int maximo = getMaxMin(A, n);
    for (int i = 0; i <= maximo; i++)
    {
        bucket[i] = 0;
    }
    for (int i = 0; i < n; i++)
    {
        bucket[A[i]]++;
    }
    for (int i = 0, j = 0; i <= maximo; i++)
    {
        while (bucket[i] > 0)
        {
            A[j++] = i;
            bucket[i]--;
        }
    }
}

void uso()
// Descricao: imprime as opcoes de uso
// Entrada: nao tem
// Saida: impressao das opcoes de linha de comando
{
    fprintf(stderr, "sortperf\n");
    fprintf(stderr, "\t-z <int>\t(vector size)\n");
    fprintf(stderr, "\t-s <int>\t(initialization seed)\n");
    fprintf(stderr, "\t-a <s|i|q|q3|qi|q3i|h|rs|ms>\t(algorithm)\n");
    fprintf(stderr, "\t    s\tselection\n");
    fprintf(stderr, "\t    i\tinsertion\n");
    fprintf(stderr, "\t    q\tquicksort\n");
    fprintf(stderr, "\t    q3\tquicksort+median3\n");
    fprintf(stderr, "\t    qi\tquicksort+insertion\n");
    fprintf(stderr, "\t    q3i\tquicksort+median3+insertion\n");
    fprintf(stderr, "\t    h\tshellsort\n");
    fprintf(stderr, "\t    rs\trecursive selection\n");
    fprintf(stderr, "\t    ms\tmerge sort\n");
    fprintf(stderr, "\t    bs\tbubble sort\n");
    fprintf(stderr, "\t    rd\tradix sort\n");
    fprintf(stderr, "\t    bk\tbucket sort\n");
    fprintf(stderr, "\t    cs\tcounting sort\n");
}

void parse_args(int argc, char **argv, opt_t *opt)
// Descricao: le as opcoes da linha de comando e inicializa variaveis
// Entrada: argc, argv, opt
// Saida: opt
{
    // variaveis externas do getopt
    extern char *optarg;
    extern int optind;

    // variavel auxiliar
    int c;

    // inicializacao variaveis globais para opcoes
    opt->seed = 1;
    opt->size = 10;
    opt->alg = 1;

    // getopt - letra indica a opcao, : junto a letra indica parametro
    // no caso de escolher mais de uma operacao, vale a ultima
    while ((c = getopt(argc, argv, "z:s:a:h")) != EOF)
    {
        switch (c)
        {
        case 'z':
            opt->size = atoi(optarg);
            break;
        case 's':
            opt->seed = atoi(optarg);
            break;
        case 'a':
            opt->alg = name2num(optarg);
            break;
        case 'h':
        default:
            uso();
            exit(1);
        }
    }
    if (!opt->alg)
    {
        uso();
        exit(1);
    }
}

void clkDiff(struct timespec t1, struct timespec t2,
             struct timespec *res)
// Descricao: calcula a diferenca entre t2 e t1, que e armazenada em res
// Entrada: t1, t2
// Saida: res
{
    if (t2.tv_nsec < t1.tv_nsec)
    {
        // ajuste necessario, utilizando um segundo de tv_sec
        res->tv_nsec = 1000000000 + t2.tv_nsec - t1.tv_nsec;
        res->tv_sec = t2.tv_sec - t1.tv_sec - 1;
    }
    else
    {
        // nao e necessario ajuste
        res->tv_nsec = t2.tv_nsec - t1.tv_nsec;
        res->tv_sec = t2.tv_sec - t1.tv_sec;
    }
}

int main(int argc, char **argv)
{
    sortperf_t s;
    int *vet;
    char buf[200];
    char pref[100];
    opt_t opt;
    struct timespec inittp, endtp, restp;
    int retp;

    // parse_args
    parse_args(argc, argv, &opt);

    // malloc with opt.size+1 for heapsort
    vet = (int *)malloc((opt.size + 1) * sizeof(int));

    // initialize
    resetcounter(&s);
    srand48(opt.seed);
    initVector(vet, opt.size);
    vet[opt.size] = vet[0]; // for heapsort
    // if (opt.size < 100) printVector(vet, opt.size);

    retp = clock_gettime(CLOCK_MONOTONIC, &inittp);

    // execute algorithm
    switch (opt.alg)
    {
    case ALGINSERTION:
        insertionSort(vet, 0, opt.size - 1, &s);
        break;
    case ALGSELECTION:
        selectionSort(vet, 0, opt.size - 1, &s);
        break;
    case ALGQSORT:
        quickSort(vet, 0, opt.size - 1, &s);
        break;
    case ALGQSORT3:
        quickSort3(vet, 0, opt.size - 1, &s);
        break;
    case ALGQSORTINS:
        quickSortIns(vet, 0, opt.size - 1, &s);
        break;
    case ALGQSORT3INS:
        quickSort3Ins(vet, 0, opt.size - 1, &s);
        break;
    case ALGSHELLSORT:
        shellSort(vet, opt.size, &s);
        break;
    case ALGRECSEL:
        recursiveSelectionSort(vet, 0, opt.size - 1, &s);
        break;
    case ALGMERGE:
        mergeSort(vet, 0, opt.size - 1, &s);
        break;
    case ALGBUBBLE:
        bubbleSort(vet, opt.size - 1, &s);
        break;
    case ALGRDX:
        radixSort(vet, opt.size, &s);
        break;
    case ALGBKT:
        bucketSort(vet, opt.size, &s);
        break;
    case ALGCOUNT:
        countingSort(vet, opt.size, opt.size, &s);
        break;
    }

    retp = clock_gettime(CLOCK_MONOTONIC, &endtp);
    clkDiff(inittp, endtp, &restp);

    // if (opt.size<100) printVector(vet, opt.size);

    // print stats
    sprintf(pref, "alg %s seed %d size %d time %ld.%.9ld",
            num2name(opt.alg), opt.seed, opt.size, restp.tv_sec, restp.tv_nsec);

    printsortperf(&s, buf, pref);
    printf("%s\n", buf);

    exit(0);
}
