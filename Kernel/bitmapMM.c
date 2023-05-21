#include "bitmapMM.h" // import 2

#include <stdint.h>
#include <stdarg.h>

uint8_t bitMap[BIT_MAP_SIZE];

// declarar un super array donde cada bit representa un bloque/pagina
// tenemos que hacer un inicializador del Memory Manager, donde se declare dicho  array
// una funcion que retorne un puntero a la memoria pedida
// una funcion que libere la memoria asignada

void initBitMap()
{
    memset(bitMap, 0, BIT_MAP_SIZE);
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
    char mask = 128; // 1000 0000
    for (bitMapPosition = 0; bitMapPosition < BIT_MAP_SIZE; bitMapPosition++)
    {
        for (bitPosition = 0; bitPosition < 8; bitPosition++)
        {
            // printf("%d--\n",!(bitMap[bitMapPosition] & mask));
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

/**
 * @brief
 *
 * @param dir direccion donde esta
 * @param size es en bits
 */
void freeBits(void *dir, int size)
{

    int dirMap = (((int)dir) - MEM_START) / PAG_SIZE; // base + 4k*(8*posArr + bitPoss) bitPos[ 0-7 ]
    int posArr = dirMap / 8;                          // se trunca
    int bitPos = dirMap % 8;                          // me da cosas del 0 al 7
    int cantPag = (size + PAG_SIZE - 1) / PAG_SIZE;
    switchBits(posArr, bitPos, cantPag); // gomensa
    return;
}

void *allocBits(int sizeBytes)
{
    void *resp;
    int posArr = 0;
    int bitPos = 0;
    int cantPag = (sizeBytes + PAG_SIZE - 1) / PAG_SIZE;
    if (findSpace(cantPag, &posArr, &bitPos))
    {
        switchBits(posArr, bitPos, cantPag);
        resp = MEM_START + PAG_SIZE * (8 * posArr + bitPos);
        return (void *)(MEM_START + PAG_SIZE * (8 * posArr + bitPos));
    }
    return 0;
}