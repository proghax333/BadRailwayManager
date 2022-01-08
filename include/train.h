
#pragma once

#include "context.h"
#include "utils.h"
#include "crud_generator.h"

typedef struct
{
  int id;
  char name[SIZE];
  char code[SIZE];
} Train;

GENERATE_CRUD(Train);

bool TrainCompareId(void *a, void *b)
{
  TrainNode *nodeA = (TrainNode *)a;
  TrainNode *nodeB = (TrainNode *)b;

  return nodeA->data.id == nodeB->data.id;
}

bool TrainCompareCode(void *a, void *b)
{
  TrainNode *nodeA = (TrainNode *)a;
  TrainNode *nodeB = (TrainNode *)b;

  return strcmp(nodeA->data.code, nodeB->data.code) == 0;
}
