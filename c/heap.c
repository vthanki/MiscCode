#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIN_HEAP	0
#define MAX_HEAP	1

typedef struct heap {
	int *arr;
	int size;
	int capacity;
	int type;
} heap_t;


heap_t *alloc_heap(int capacity)
{
	heap_t *heap = (heap_t *)malloc(sizeof(heap_t));
	heap->capacity = capacity;
	heap->arr = (int *)malloc(sizeof(int) * heap->capacity);
	heap->size = 0;
	heap->type = MIN_HEAP;
	return heap;
}

void free_heap(heap_t *heap)
{
	free(heap->arr);
	free(heap);
}

void increase_heap(heap_t *heap)
{
	int *arr = (int *)malloc(heap->capacity*2*sizeof(int));

	heap->capacity *= 2;
	memset(arr, 0x0, sizeof(int)*heap->capacity);
	memcpy(arr, heap->arr, sizeof(int)*(heap->capacity/2));
	free(heap->arr);
	heap->arr = arr;
}

int get_parent_idx(int idx)
{
	return (idx - 1)/2;
}

int get_lchild_idx(int idx)
{
	return idx*2 + 1;
}

int get_rchild_idx(int idx)
{
	return idx*2 + 2;
}

int get_parent(heap_t *heap, int idx)
{
	int pidx = (idx - 1)/2;
	return heap->arr[pidx];
}

int get_lchild(heap_t *heap, int idx)
{
	return heap->arr[idx*2 + 1];
}

int get_rchild(heap_t *heap, int idx)
{
	return heap->arr[idx*2 + 2];
}

int has_lchild(heap_t *heap, int idx)
{
	return (idx*2+1) < heap->size;
}

int has_rchild(heap_t *heap, int idx)
{
	return (idx*2+2) < heap->size;
}

int has_parent(heap_t *heap, int idx)
{
	return (idx != 0) && ((idx-1)/2) >= 0;
}

void swap(heap_t *heap, int idx1, int idx2)
{
	int temp = heap->arr[idx1];
	heap->arr[idx1] = heap->arr[idx2];
	heap->arr[idx2] = temp;
}

void adjust_up(heap_t *heap)
{
	int curr_idx = heap->size - 1;

	while (has_parent(heap, curr_idx) &&
		(get_parent(heap, curr_idx) > heap->arr[curr_idx])) {
		swap(heap, get_parent_idx(curr_idx), curr_idx);
		curr_idx = get_parent_idx(curr_idx);
	}
//	for (int i = 0; i < 4; i++)	printf("%04d ", heap->arr[i]);
//	printf("%s: curr_idx:%d, parent_idx:%d, parent:%d top:%d has_parent:%d\n", __func__, curr_idx,
//			get_parent_idx(curr_idx), get_parent(heap, curr_idx), heap->arr[curr_idx], has_parent(heap, curr_idx));
}

void push(heap_t *heap, int value)
{
	if (heap->size == heap->capacity)
		increase_heap(heap);

	heap->arr[heap->size] = value;
	heap->size++;

	adjust_up(heap);
}

void adjust_down(heap_t *heap)
{
	int curr_idx = 0;
	int min_child_idx;
	while (has_lchild(heap, curr_idx)) {
		min_child_idx = get_lchild_idx(curr_idx);
		if (has_rchild(heap, curr_idx) &&
			(heap->arr[min_child_idx] > get_rchild(heap, curr_idx)))
			min_child_idx = get_rchild_idx(curr_idx);

		if (heap->arr[min_child_idx] < heap->arr[curr_idx]) {
			swap(heap, min_child_idx, curr_idx);
			curr_idx = min_child_idx;
		} else {
			break;
		}
	}
}

int pop(heap_t *heap)
{
	int ret;

	if (heap->size <= 0)
		return -1;

	ret = heap->arr[0];
	heap->arr[0] = heap->arr[heap->size-1];
	heap->size--;

	adjust_down(heap);

	return ret;
}

int main()
{
	heap_t *min_heap = alloc_heap(32);
	int a[] = {2,5,2,9,7,2,1,4,5,6,7};
	int ret;
	for (int i = 0; i < sizeof(a)/sizeof(a[0]); i++) {
		push(min_heap, a[i]);
	}

	while ((ret = pop(min_heap)) >= 0)
		printf("%d ", ret);

	printf("\n");
	return 0;
}
