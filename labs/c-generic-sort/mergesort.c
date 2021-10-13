void merge(void *lineptr[], int left, int mid, int right, int (*comp)(void *, void *))
{
    int i, j, k;
    int s1 = mid - left + 1;
    int s2 = right - mid;
    void *LEFT[s1];
	void *RIGHT[s2];
    for (i = 0; i < s1; i++) {
			LEFT[i] = lineptr[left + i];
		}
    for (j = 0; j < s2; j++) {
			RIGHT[j] = lineptr[mid + 1 + j];
		}
    i = 0;
    j = 0; 
    k = left; 
    while (i < s1 && j < s2) {
        if ((*comp)(LEFT[i], RIGHT[j]) <= 0) {
            lineptr[k] = LEFT[i];
            i++;
        }
        else {
            lineptr[k] = RIGHT[j];
            j++;
        }
        k++;
    }
  
    while (i < s1) {
        lineptr[k] = LEFT[i];
        i++;
        k++;
    }
  
    while (j < s2) {
        lineptr[k] = RIGHT[j];
        j++;
        k++;
    }
}
  
void mergesort(void *lineptr[], int left, int right, int (*comp)(void *, void *)) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergesort(lineptr, left, mid, comp);
        mergesort(lineptr, mid + 1, right, comp);
        merge(lineptr, left, mid, right, comp);
    }
}