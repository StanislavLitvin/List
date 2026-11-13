#include "List.h"
#include "List_Functions.cpp"

int main()
{
  Clear_Log_File();
  
  struct List Lst = {0};
  List_Init(&Lst);
  
  List_Push_To_Head(&Lst, 10);// Сейчас код не работает, проблема в проверке на зацикливание в варификаторе
  List_Push_To_Head(&Lst, 20);
  List_Push_To_Tail(&Lst, 30);
  List_Push_To_Tail(&Lst, 40);
  List_Push_To_Index(&Lst, 2, 25);// БЛЯТЬ!!! ТЫ ОБЪЕБАЛСЯ ЖЕСТКО! НАДО ВЫЗЫВАТЬ НЕ ОТ НОМЕРА В СПИСКЕ, А ОТ ID (ИНДЕКСА В ФИЗИЧЕСКОМ МАССИВЕ)

  List_Dump(&Lst, WITH_DUMPING_ARRAYS);

  List_Elements Val1 = List_Pop_From_Head(&Lst);
  List_Elements Val2 = List_Pop_From_Tail(&Lst);
  List_Elements Val3 = List_Pop_From_Index(&Lst, 1);

  printf("Popped: %lld, %lld, %lld\n\n", Val1, Val2, Val3);

  List_Dump(&Lst, WITH_DUMPING_ARRAYS);

  List_Destroy(&Lst);

  return 0;
}