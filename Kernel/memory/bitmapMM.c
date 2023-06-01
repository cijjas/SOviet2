#ifndef BUDDY_MM

#include "memoryManager.h"


uint8_t bitMap[BIT_MAP_SIZE];
void *memStart;
uint32_t memSize;

uint32_t allocatedBytes; // en bytes

typedef struct
{
    void *address;
    size_t size;
} Allocation;
Allocation allocations[BIT_MAP_SIZE];
int numAllocations = 0;

void initMemoryManager(void *hBase, uint32_t hSize)
{
    if (hBase == NULL || hSize == 0)
    {
        return;
    }
    memStart = (uint32_t)hBase;
    memSize = hSize;

    memset(bitMap, 0, BIT_MAP_SIZE);
    memset(allocations, 0, BIT_MAP_SIZE);
    numAllocations = 0;
}

void switchBit(char *ch, int bitPos)
{
    *ch ^= (1 << bitPos);
}

// funcion que switchea el valor del bit que se encuentra en position dentro del byte apuntado por byteDir
void switchBits(int posArr, int bitPos, int size)
{
    // ejemplo switchBits(8, 5, 14); se quiere switchear los 15 bits comenzando del 8-5 al 10-4
    unsigned char mask = 128 >> bitPos;          // la mascara se setea en 0000 0100
    for (int i = bitPos; i < bitPos + size; i++) // i = 5 -> 19
    {
        int arrStep = i / 8; //  0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2
        if (i % 8 == 0)
        {
            mask = 128;
        }
        switchBit(&bitMap[posArr + arrStep], 7 - i % 8);
        mask = mask >> 1;
    }
    return;
}

// funcion que retorna la direccion donde comienza el bloque con el tamanio requerido
int findSpace(int cantPag, int *posArr, int *bitPos)
{
    int freeSpace = 0; // variable que acumula la cantidad de memoria que se encontro hasta el momento
    int bitMapPosition;
    char bitPosition;
    int startArrPos = 0;
    char startBitPos = 0;
    for (bitMapPosition = 0; bitMapPosition < BIT_MAP_SIZE; bitMapPosition++)
    {
        unsigned char mask = 128; // 1000 0000
        for (bitPosition = 0; bitPosition < 8; bitPosition++)
        {
            // bitMap[bitMapPosition] = 1100 0001
            // bitPosition 1000 0000 -> 0100 0000 -> 0010 0000 -> 0001 0000 -> 0000 1000 -> 0000 0100 -> 0000 0010 -> 0000 0001
            if (freeSpace == 0)
            {
                startArrPos = bitMapPosition;
                startBitPos = bitPosition;
            }
            if ((bitMap[bitMapPosition] & mask))
            {
                freeSpace = 0;
            }
            else
            {
                freeSpace++;
            }
            if (freeSpace == cantPag)
            {
                *posArr = startArrPos;
                *bitPos = startBitPos;
                return 1;
            }
            mask = mask >> 1;
        }
    }
    return 0;
}

void memFree(void *dir)
{
    for (int i = 0; i < numAllocations; i++)
    {
        if (allocations[i].address == dir)
        {
            uint64_t dirMap = (((uint64_t)dir) - (uint64_t)(memStart)) / PAG_SIZE; // base + 4k*(8*posArr + bitPoss) bitPos[ 0-7 ]
            int posArr = dirMap / 8;                                               // se trunca
            int bitPos = dirMap % 8;
            int cantPag = (allocations[i].size + PAG_SIZE - 1) / PAG_SIZE;
            switchBits(posArr, bitPos, cantPag);

            // Remove deallocated memory from allocations array
            for (int j = i; j < numAllocations; j++)
            {
                allocations[j] = allocations[j + 1];
            }
            allocatedBytes -= cantPag * PAG_SIZE;
            numAllocations--;

            return;
        }
    }
}

void *memAlloc(int sizeBytes)
{
    int posArr = 0;
    int bitPos = 0;
    int cantPag = (sizeBytes + PAG_SIZE - 1) / PAG_SIZE;
    if (findSpace(cantPag, &posArr, &bitPos))
    {
        switchBits(posArr, bitPos, cantPag);
        void *address = (memStart + PAG_SIZE * (posArr * 8 + bitPos));
        allocations[numAllocations].address = address;
        allocations[numAllocations].size = sizeBytes;
        numAllocations++;
        allocatedBytes += cantPag * PAG_SIZE;
        //print("POS ARRAY [%d] BIT POS [%d]\n", posArr, bitPos);
        return address;
    }
    return 0;
}


char * mem(int unit) // crea string de memoria total, ocupada y libre
{ 
  size_t memSize =  8 *BIT_MAP_SIZE * PAG_SIZE; 
  size_t total;
  size_t allocated;
  size_t free;
  char* memStateString;

  if(unit == 0){ // mb
    size_t kibiConvert = 1024*1024;
    total = (size_t) memSize/kibiConvert; 
    allocated = allocatedBytes/kibiConvert; 
    free = (total - allocated); 
    memStateString = snprintf( "Estado de la Memoria\n %d MB de memoria total\n %d MB en uso\n %d MB libres\n Para mayor precision usar el comando 'memb'\n", total, allocated, free);
  }
  else if(unit == 1){ // bytes
    total = (size_t) memSize; 
    allocated = allocatedBytes; 
    free = (total - allocated); 
    memStateString = snprintf( "Estado de la Memoria\n %d Bytes de memoria total\n %d Bytes en uso\n %d Bytes libres\n", total, allocatedBytes, free);
  }
  else{
    memStateString = snprintf( "Unidad no reconocida\n");
  }
  return memStateString;
}

#endif
