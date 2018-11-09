def ver(l):
    print("~~datos a ingresar")
    for i in l:
        print(i)
    print()

def uphead(arreglo,pos,cmp):
    if pos == 0 : return

    padre = (pos - 1) // 2

    # padre > hijo
    if cmp (arreglo[padre], arreglo[pos]) > 0:

        #swap
        arreglo[padre],arreglo[pos] = arreglo[pos], arreglo[padre]
        uphead(arreglo,padre,cmp)

def heap_encolar(arreglo,dato,cmp):
    arreglo.append(dato)
    uphead(arreglo,len(arreglo)-1,cmp)

def downheap(arreglo, n, padre, cmp):
    if padre >= n : return

    min = padre
    izq = (2 * padre) + 1
    der = (2 * padre) + 2

    if izq < n and cmp(arreglo[izq], arreglo[min]) < 0 : min = izq

    if der < n and cmp(arreglo[der], arreglo[min]) < 0 : min = der

    if min != padre:
        #swap
        arreglo[padre],arreglo[min] = arreglo[min], arreglo[padre]
        downheap(arreglo, n , min, cmp)

def heap_desencolar(arreglo,cmp):
    if not arreglo: return

    #swap
    arreglo[0],arreglo[len(arreglo)-1] = arreglo[len(arreglo)-1], arreglo[0]

    dato = arreglo.pop()

    downheap(arreglo, len(arreglo), 0, cmp)

    return dato


def cmp_minimos(a,b):
    #if v[0] == v2[0]:
        #if v[1] < v2[1] : return 1
        #if v[1] > v2[1] : return -1
        #return 0

    if a[0] == b[0]:
        if a[1] < b[1] : return -1
        if a[1] > b[1] : return 1
        return 0

    #min function
    #return -1 if a[0] < b[0] else 1
    return 1 if a[0] < b[0] else -1

def main():
    arreglo = []

    lista = [[2,3],[99,7],[99,1],[1,52],[19,1],[1,1],[10,1234],[3,325]]
    testCatedra = [[3,324],[10,1234],[3,325]]

    ver(testCatedra)

    for i in testCatedra:
        #h2.encolar(i)
        heap_encolar(arreglo,i,cmp_minimos)

    print("result:")
    for r in testCatedra :
        print("\t",heap_desencolar(arreglo,cmp_minimos))

    print("\n\n~~~TEST TERMINADO~~~")


main()
