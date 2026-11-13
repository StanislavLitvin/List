typedef long long int List_Elements;

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define FILE_FOR_ERRORS "Dumb_File.log"
#define FILE_FOR_DUMP "MEOW.txt"
#define START_CELL_POISON (List_Elements)0xBAADEDAA
#define POISON (List_Elements)0xDEADDEAD

const int LIST_CAPACITY = 16;
const List_Elements ERROR_STATUS_LIST_POP = -1000;

enum List_Error_Status
{
  EVERYTHING_OK          =  0,
  MEMORY_SHORTAGE        = -1,
  LIST_IS_OVERFLOW       = -2,
  INCORRECT_INSERT_INDEX = -3,
  ZERO_LIST_POINTER      = -4,
  INVALID_POIZON_VALUE   = -5,
  INVALID_SIZE           = -6,
  INVALID_HEAD           = -7,
  INVALID_TAIL           = -8,
  ERROR_OPENING_FILE     = -9,
  ERROR_DUMP             = -10,
  INVALID_INDEX          = -11,
  LIST_IS_CYCLIC         = -12,
  ERROR_DETECTING_CYCLE  = -13
};

enum Mode_Dump
{
  WITHOUT_ANYTHING       = 1,
  WITHOUT_DUMPING_ARRAYS = 2,
  WITH_DUMPING_ARRAYS    = 3
};

struct List
{
  List_Elements* Data;
  int* Next;
  int* Prev;
  int Head;
  int Tail;
  int Size;
  int Capacity;
  int Free;
};

void Clear_Log_File();
int Find_Free_Cell(struct List* Lst);
List_Error_Status List_Init(struct List* Lst);
void List_Destroy          (struct List* Lst);

List_Error_Status List_Push_To_Head (struct List* Lst, List_Elements Value);
List_Error_Status List_Push_To_Tail (struct List* Lst, List_Elements Value);
List_Error_Status List_Push_To_Index(struct List* Lst, int Index, List_Elements Value);

List_Elements List_Pop_From_Head (struct List* Lst);
List_Elements List_Pop_From_Tail (struct List* Lst);
List_Elements List_Pop_From_Index(struct List* Lst, int Index);

List_Error_Status List_Verify       (struct List* Lst);
List_Error_Status List_Dump         (struct List* Lst, Mode_Dump Mode);
List_Error_Status List_Dump_Text    (struct List* Lst, Mode_Dump Mode);
List_Error_Status List_Dump_Graphviz(struct List* Lst);

List_Error_Status List_Detecting_Cycle(struct List* Lst);