#include "../Headers/Heap.h"

void maxheapify(  VariableBox *array, int parent, int sizeofHeap)
{
    int largest, lchild, rchild;
    struct VariableBox temp;
    lchild = (2*parent)+1;
    rchild = ((2*parent)+2);
    //check if there is left child
    if(lchild>=sizeofHeap)
        return;

    if (lchild < sizeofHeap && array[lchild].value> array[parent].value)
    {
        largest = lchild;
    }

    else
    {
        largest = parent;
    }
    if (rchild < sizeofHeap && array[rchild].value > array[largest].value)
    {
        largest = rchild;
    }

    if (largest != parent)
    {
        temp.value = array[ parent].value;
        temp.key=array[parent].key;
        strcpy( temp.type,array[parent].type);

        array[ parent].value = array[largest].value;
        array[ parent].key = array[largest].key;
        strcpy(array[ parent].type ,array[largest].type);

        array[largest].value = temp.value;
        array[largest].key = temp.key;
        strcpy(array[largest].type ,temp.type);
        maxheapify(array, largest, sizeofHeap);

    }
}

VariableBox* buildmaxheap(VariableBox *a, int n)
{
    int j;
    VariableBox temp;
    for (j = n/2-1; j >=0 ; j--)
    {
        maxheapify(a, j, n);
    }int i;
    for (i=n-1; i>0; i--)
    {
        temp.value = a[0].value;
        temp.key = a[0].key;
        strcpy(temp.type, a[0].type);

        a[0].value=a[i].value;
        a[0].key=a[i].key;
        strcpy(a[0].type, a[i].type);

        a[i].value = temp.value;
        a[i].key = temp.key;
        strcpy(a[i].type,temp.type);
        maxheapify(a, 0, i);
    }
    return a;

}
void displayHeap( VariableBox *a,int n) {

    for (int i = 0;i < n; i++) {

        if (!strcmp(a[i].type, "int"))
            printf(ANSI_COLOR_BOLD_BLUE "Key: %s Value: %d Type: %s\n" ANSI_COLOR_RESET, a[i].key, (int) a[i].value, a[i].type);
        else
            printf(ANSI_COLOR_BOLD_BLUE "Key: %s Value: %f Type: %s\n" ANSI_COLOR_RESET , a[i].key, a[i].value, a[i].type);

    }

}