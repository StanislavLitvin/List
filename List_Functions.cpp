void Clear_Log_File()
{
  FILE* Clear_File = fopen(FILE_FOR_ERRORS, "w");
  if (Clear_File != NULL)
    fclose(Clear_File);

  return;
}

int Find_Free_Cell(struct List* Lst)
{
  assert(List_Verify(Lst) == EVERYTHING_OK);

  for (int i = 1; i < Lst->Capacity; i++)
  {
    if (Lst->Data[i] == POISON)
      return i;
  }

  assert(List_Verify(Lst) == EVERYTHING_OK);

  return LIST_IS_OVERFLOW;
}

List_Error_Status List_Init(struct List* Lst)
{
  assert(Lst);

  Lst->Capacity = LIST_CAPACITY;
  Lst->Size = 0;
  Lst->Free = 0;

  List_Elements* Pointer_Data = (List_Elements*)calloc((size_t)Lst->Capacity,
                                                       sizeof(List_Elements));
  int* Pointer_Next = (int*)calloc((size_t)Lst->Capacity, sizeof(int));
  int* Pointer_Prev = (int*)calloc((size_t)Lst->Capacity, sizeof(int));

  if (Pointer_Data == NULL || Pointer_Next == NULL || Pointer_Prev == NULL)
  {
    printf("ERROR: MEMORY_SHORTAGE\n");
    return MEMORY_SHORTAGE;
  }

  Pointer_Data[0] = START_CELL_POISON;
  Pointer_Next[0] = 0;
  Pointer_Prev[0] = 0;
  for (int i = 1; i < LIST_CAPACITY; i++)
  {
    Pointer_Data[i] = POISON;
    Pointer_Next[i] = -1;
    Pointer_Prev[i] = -1;
  }

  Lst->Data = Pointer_Data;
  Lst->Next = Pointer_Next;
  Lst->Prev = Pointer_Prev;
  Lst->Head = 0;
  Lst->Tail = 0;

  return EVERYTHING_OK;
}

void List_Destroy(struct List* Lst)
{
  assert(Lst);

  Lst->Data[0] = START_CELL_POISON;
  Lst->Next[0] = 0;
  Lst->Prev[0] = 0;

  for (int i = 1; i < Lst->Capacity; i++)
  {
    Lst->Data[i] = POISON;
    Lst->Next[i] = -1;
    Lst->Prev[i] = -1;
  }

  free(Lst->Data);
  free(Lst->Next);
  free(Lst->Prev);
  Lst->Data = NULL;
  Lst->Next = NULL;
  Lst->Prev = NULL;
  Lst->Head = 0;
  Lst->Tail = 0;
  Lst->Capacity = 0;
  Lst->Size = 0;
  Lst->Free = 0;

  return;
}

List_Error_Status List_Push_To_Head(struct List* Lst, List_Elements Value)
{
  assert(List_Verify(Lst) == EVERYTHING_OK);
  
  int Free_Index = Find_Free_Cell(Lst);
  if (Free_Index == LIST_IS_OVERFLOW)
  {
    printf("ERROR: can't push to list (list is overflow)\n");
    return LIST_IS_OVERFLOW;
  }

  Lst->Data[Free_Index] = Value;
  Lst->Next[Free_Index] = 0;
  Lst->Prev[Free_Index] = Lst->Head;

  if (Lst->Size != 0)
    Lst->Next[Lst->Head] = Free_Index;
  else
    Lst->Tail = Free_Index;

  Lst->Head = Free_Index;
  (Lst->Size)++;

  assert(List_Verify(Lst) == EVERYTHING_OK);

  return EVERYTHING_OK;
}

List_Error_Status List_Push_To_Tail(struct List* Lst, List_Elements Value)
{
  assert(List_Verify(Lst) == EVERYTHING_OK);
  
  int Free_Index = Find_Free_Cell(Lst);
  if (Free_Index == LIST_IS_OVERFLOW)
  {
    printf("ERROR: can't push to list (list is overflow)\n");
    return LIST_IS_OVERFLOW;
  }

  Lst->Data[Free_Index] = Value;
  Lst->Next[Free_Index] = Lst->Tail;
  Lst->Prev[Free_Index] = 0;

  if (Lst->Size != 0)
    Lst->Prev[Lst->Tail] = Free_Index;
  else
    Lst->Head = Free_Index;

  Lst->Tail = Free_Index;
  (Lst->Size)++;

  assert(List_Verify(Lst) == EVERYTHING_OK);

  return EVERYTHING_OK;
}

List_Error_Status List_Push_To_Index(struct List* Lst, int Index, List_Elements Value)
{
  assert(List_Verify(Lst) == EVERYTHING_OK);

  if (Index < 0 || Index > Lst->Size)
  {
    printf("ERROR: Insert index is too small or large\n");
    return INCORRECT_INSERT_INDEX;
  }

  else if (Index == Lst->Size || Lst->Size == 0)
    return List_Push_To_Head(Lst, Value);

  else if (Index == 0)
    return List_Push_To_Tail(Lst, Value);

  else
  {
    int Free_Index = Find_Free_Cell(Lst);
    if (Free_Index == LIST_IS_OVERFLOW)
    {
      printf("ERROR: can't push to list (list is overflow)\n");
      return LIST_IS_OVERFLOW;
    }

    int Current = Lst->Tail;
    for (int i = 0; i < Index; i++)
    {
      Current = Lst->Next[Current]; // o(1)!!!
    }

    Lst->Data[Free_Index] = Value;
    Lst->Next[Free_Index] = Current;
    Lst->Next[Lst->Prev[Current]] = Free_Index;
    Lst->Prev[Free_Index] = Lst->Prev[Current];
    Lst->Prev[Current] = Free_Index;

    (Lst->Size)++;
  }

  assert(List_Verify(Lst) == EVERYTHING_OK);

  return EVERYTHING_OK;
}

List_Elements List_Pop_From_Head(struct List* Lst)
{
  assert(List_Verify(Lst) == EVERYTHING_OK);

  if (Lst->Size == 0)
    return ERROR_STATUS_LIST_POP;

  int Temp_Index = Lst->Head;
  List_Elements Value = Lst->Data[Temp_Index];
  
  Lst->Head = Lst->Prev[Temp_Index];

  if (Lst->Head != 0)
    Lst->Next[Lst->Head] = 0;

  (Lst->Size)--;

  if (Lst->Size == 0)
    Lst->Tail = 0;

  Lst->Data[Temp_Index] = POISON;
  Lst->Next[Temp_Index] = 0;
  Lst->Prev[Temp_Index] = 0;

  assert(List_Verify(Lst) == EVERYTHING_OK);

  return Value;
}

List_Elements List_Pop_From_Tail(struct List* Lst)
{
  assert(List_Verify(Lst) == EVERYTHING_OK);

  if (Lst->Size == 0)
    return ERROR_STATUS_LIST_POP;

  int Temp_Index = Lst->Tail;
  List_Elements Value = Lst->Data[Temp_Index];
  
  Lst->Tail = Lst->Next[Temp_Index];

  if (Lst->Tail != 0)
    Lst->Prev[Lst->Tail] = 0;

  (Lst->Size)--;

  if (Lst->Size == 0)
    Lst->Head = 0;

  Lst->Data[Temp_Index] = POISON;
  Lst->Next[Temp_Index] = 0;
  Lst->Prev[Temp_Index] = 0;

  assert(List_Verify(Lst) == EVERYTHING_OK);

  return Value;
}

List_Elements List_Pop_From_Index(struct List* Lst, int Index)
{
  assert(List_Verify(Lst) == EVERYTHING_OK);

  if (Index < 0 || Index > Lst->Size)
  {
    printf("ERROR: Insert index is too small or large\n");
    return ERROR_STATUS_LIST_POP;
  }

  if (Lst->Size == 0)
    return ERROR_STATUS_LIST_POP;

  if (Index == Lst->Size)
    return List_Pop_From_Head(Lst);

  else if (Index == 0)
    return List_Pop_From_Tail(Lst);

  else
  {
    int Current = Lst->Tail;
    for (int i = 0; i < Index; i++)
    {
      Current = Lst->Next[Current];
    }

    List_Elements Value = Lst->Data[Current];

    Lst->Next[Lst->Prev[Current]] = Lst->Next[Current];
    Lst->Prev[Lst->Next[Current]] = Lst->Prev[Current];

    Lst->Data[Current] = POISON;
    Lst->Next[Current] = 0;
    Lst->Prev[Current] = 0;

    (Lst->Size)--;

    assert(List_Verify(Lst) == EVERYTHING_OK);

    return Value;
  }
}

List_Error_Status List_Verify(struct List* Lst)
{
  if (Lst == NULL)
    return List_Dump_Text(Lst, WITHOUT_ANYTHING);

  else if (Lst->Data == NULL || Lst->Next == NULL || Lst->Prev == NULL)
  {
    List_Dump_Text(Lst, WITHOUT_DUMPING_ARRAYS);
    printf("ERROR: ZERO_LIST_POINTER\n");
    return ZERO_LIST_POINTER;
  }

  else if (Lst->Data[0] != START_CELL_POISON || Lst->Next[0] != 0 || Lst->Prev[0] != 0)
  {
    List_Dump(Lst, WITH_DUMPING_ARRAYS);
    printf("ERROR: INVALID_POIZON_VALUE\n");
    return INVALID_POIZON_VALUE;
  }

  else if (Lst->Size < 0 || Lst->Size > Lst->Capacity)
  {
    List_Dump(Lst, WITH_DUMPING_ARRAYS);
    printf("ERROR: INVALID_SIZE\n");
    return INVALID_SIZE;
  }

  else if (Lst->Head < 0 || Lst->Head >= Lst->Capacity)
  {
    List_Dump(Lst, WITH_DUMPING_ARRAYS);
    printf("ERROR: INVALUD_HEAD\n");
    return INVALID_HEAD;
  }

  else if (Lst->Tail < 0 || Lst->Tail >= Lst->Capacity)
  {
    List_Dump(Lst, WITH_DUMPING_ARRAYS);
    printf("ERROR: INVALUD_TAIL\n");
    return INVALID_TAIL;
  }

  else
  {
    if (List_Detecting_Cycle(Lst) != EVERYTHING_OK) //—ƒ≈Ћј“№ Ќќ–ћјЋ№Ќќ!
      return ERROR_DETECTING_CYCLE;
  }
  
  List_Dump(Lst, WITH_DUMPING_ARRAYS);
  printf("ERROR: EVERYTHING_OK\n");
  return EVERYTHING_OK;
}

List_Error_Status List_Dump(struct List* Lst, Mode_Dump Mode)
{
  switch(Mode)
  {
    case WITHOUT_ANYTHING:
      if (List_Dump_Text(Lst, Mode) != EVERYTHING_OK)
        return ERROR_DUMP;
      break;

    case WITHOUT_DUMPING_ARRAYS:
      if (List_Dump_Text(Lst, Mode) != EVERYTHING_OK)
        return ERROR_DUMP;
      break;

    case WITH_DUMPING_ARRAYS:
      if (List_Dump_Text(Lst, Mode) != EVERYTHING_OK || List_Dump_Graphviz(Lst) != EVERYTHING_OK)
        return ERROR_DUMP;
      break;

    default:
      return ERROR_DUMP;
      break;
  }
  
  return EVERYTHING_OK;
}

List_Error_Status List_Dump_Text(struct List* Lst, Mode_Dump Mode)
{
  if (Mode == WITHOUT_ANYTHING)
  {
    printf("ERROR FROM DUMP: pointer to Lst = 0\n");
    return ZERO_LIST_POINTER;
  }

  FILE* Error_File = fopen(FILE_FOR_ERRORS, "w");
  if (Error_File == NULL)
  {
    printf("ERROR FROM DUMP: error file can't open\n");
    return ERROR_OPENING_FILE;
  }

  fprintf(Error_File, "\n-------------------List_Dump---Start----------------\n\n");
  fprintf(Error_File, "Size: %d, Capacity: %d\n", Lst->Size, Lst->Capacity);
  fprintf(Error_File, "Head: %d, Tail: %d\n\n", Lst->Head, Lst->Tail);

  if (Mode == WITH_DUMPING_ARRAYS)
  {
    fprintf(Error_File, "Index |     Data     | Next | Prev \n");
    fprintf(Error_File, "---------------------------------- \n");
    for (int i = 0; i < Lst->Capacity; i++)
    {
      if (Lst->Data[i] == START_CELL_POISON)
        fprintf(Error_File, "  %2d  | START_POISON |  %2d  |  %2d \n", i, Lst->Next[i],
                                                                          Lst->Prev[i]);
      else if (Lst->Data[i] == POISON)
        fprintf(Error_File, "  %2d  | POISON       |  %2d  |  %2d \n", i, Lst->Next[i],
                                                                          Lst->Prev[i]);
      else
        fprintf(Error_File, "  %2d  | %-12lld |  %2d  |  %2d \n", i, Lst->Data[i],
                                                                     Lst->Next[i],
                                                                     Lst->Prev[i]);
    }
  }
  fprintf(Error_File, "\n-------------------List_Dump---End----------------\n\n\n");
  
  if (fclose(Error_File) != 0)
    printf("WARNING FROM DUMP: error file can't close\n");

  return EVERYTHING_OK;
}

List_Error_Status List_Dump_Graphviz(struct List* Lst) //—делать в html-формате (просто в конце работы программы все загружать в html файл)
{
  assert(Lst);
  assert(Lst->Data);
  assert(Lst->Next);
  assert(Lst->Prev);

  FILE* Dump_File = fopen(FILE_FOR_DUMP, "w");
  if (Dump_File == NULL)
  {
    printf("ERROR FROM DUMP: dump file can't open\n");
    return ERROR_OPENING_FILE;
  }

  fprintf(Dump_File, "digraph List_Dump_Graphviz\n"
                     "{\n"
                     "\trankdir=LR;\n"
                     "\tnode [shape=Mrecord, fontcolor=\"#220808\"];\n\n"
                     "\tbgcolor=\"#5D0082\";\n"
                     "\tmargin=0;\n"
                     "\tpad=0;\n\n");// —делать под небольшое количество fprintf
  
  fprintf(Dump_File, "\tsubgraph cluster_list {\n");
  fprintf(Dump_File, "\t\tlabel=<<B><FONT POINT-SIZE=\"40\">List Dump Graphviz</FONT></B>>;\n");
  fprintf(Dump_File, "\t\tstyle=\"rounded,filled\";\n");
  fprintf(Dump_File, "\t\tfillcolor=\"#C0BAFF\";\n");
  fprintf(Dump_File, "\t\tcolor=\"#5D0082\";\n");
  fprintf(Dump_File, "\t\tpenwidth=10;\n");
  fprintf(Dump_File, "\t\tmargin=30;\n\n");

  fprintf(Dump_File, "\t\tHEAD [shape=Mrecord, style=filled, fillcolor=\"#D49B45\", "
                     "label=\"HEAD | %d\"];\n", Lst->Head);
  fprintf(Dump_File, "\t\tTAIL [shape=Mrecord, style=filled, fillcolor=\"#D49B45\", "
                     "label=\"TAIL | %d\"];\n\n", Lst->Tail);

  for (int i = 0; i < Lst->Capacity; i++)
  {
    const char* Style_Str = "";
    const char* Fillcolor_Str = "";
    const char* Data_Str = "";
    char Data_buf[64] = {};

    if (i == 0)
    {
      Style_Str = "style=filled";
      Fillcolor_Str = "fillcolor=\"#544E4E\"";
      Data_Str = "START_POISON";
    }
    else if (Lst->Data[i] == POISON)
    {
      if (Lst->Next[i] == 0 || Lst->Prev[i] == 0)
      {
        Style_Str = "style=filled";
        Fillcolor_Str = "fillcolor=\"#BD6C8B\"";
        Data_Str = "POISON";
      }
      else
      {
        Style_Str = "style=filled";
        Fillcolor_Str = "fillcolor=\"#907A7A\"";
        Data_Str = "POISON";
      }
    }
    else
    {
      Style_Str = "style=filled";
      if (Lst->Next[i] == 0 && Lst->Prev[i] == 0)
        Fillcolor_Str = "fillcolor=\"#EF9A9A\"";
      else
        Fillcolor_Str = "fillcolor=\"#93C996\"";

      snprintf(Data_buf, sizeof(Data_buf), "%lld", Lst->Data[i]);
      Data_Str = Data_buf;
    }

    fprintf(Dump_File, "\t\tnode_%d [label=\"INDEX: %d | VALUE: %s | "
                       "{PREV: %d | NEXT: %d}\", %s, %s];\n\n",
            i, i, Data_Str, Lst->Prev[i], Lst->Next[i], Style_Str, Fillcolor_Str);
  }

  fprintf(Dump_File, "\t\tTAIL -> HEAD [style=invis];\n");
  fprintf(Dump_File, "\t\tHEAD -> node_0 [style=invis];\n");

  for (int i = 0; i < Lst->Capacity - 1; i++)
    fprintf(Dump_File, "\t\tnode_%d -> node_%d [style=invis];\n", i, i + 1);

  fprintf(Dump_File, "\t}\n\n");

  if (Lst->Size != 0)
    fprintf(Dump_File, "\tHEAD -> node_%d [color=\"#8F5600\", penwidth=3, "
                       "constraint=false];\n", Lst->Head);

  if (Lst->Size != 0)
    fprintf(Dump_File, "\tTAIL -> node_%d [color=\"#8F5600\", penwidth=3, "
                       "constraint=false];\n\n", Lst->Tail);

  for (int i = 0; i < Lst->Capacity; i++)
  {
    if (Lst->Next[i] != 0 && Lst->Next[i] != -1)
    {
      if (Lst->Data[i] != POISON && Lst->Data[Lst->Next[i]] != POISON)
        fprintf(Dump_File, "\tnode_%d -> node_%d [color=\"#2C9731\", penwidth=3, "
                           "constraint=false];\n", i, Lst->Next[i]);

      else
        fprintf(Dump_File, "\tnode_%d -> node_%d [penwidth=3, constraint=false];\n",
                i, Lst->Next[i]);
    }
  }

  fprintf(Dump_File, "}\n");

  if (fclose(Dump_File) != 0)
    printf("WARNING FROM DUMP: dump file can't close\n");

  system("dot -Gpad=0 -Gmargin=0 MEOW.txt -Tpng -o MEOW.png");
  system("start MEOW.png");
  getchar();

  return EVERYTHING_OK;
}

List_Error_Status List_Detecting_Cycle(struct List* Lst)
{
  int Current = Lst->Tail;
  int Cheking_Index = 0;

  while(Current != 0 && Current < Lst->Size)
  {
    Current = Lst->Next[Current];

    if (Current < 0 || Current >= Lst->Capacity)
    {
      List_Dump(Lst, WITH_DUMPING_ARRAYS);
      printf("ERROR: INVALUD_INDEX\n");
      return INVALID_INDEX;
    }

    Cheking_Index++;
  }

  if (Current != 0)
  {
    List_Dump(Lst, WITH_DUMPING_ARRAYS);
    printf("ERROR: LIST_IS_CYCLIC\n");
    return LIST_IS_CYCLIC;
  }

  return EVERYTHING_OK;
}