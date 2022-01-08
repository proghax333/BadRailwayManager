
#pragma once

#include "context.h"
#include "utils.h"
#include "crud_generator.h"

typedef struct
{
  int id;
  int userId;
  int trainId;
  int quantity;
  int paid, total;
} Ticket;

GENERATE_CRUD(Ticket);

bool TicketCompareId(void *a, void *b)
{
  TicketNode *nodeA = (TicketNode *)a;
  TicketNode *nodeB = (TicketNode *)b;

  return nodeA->data.id == nodeB->data.id;
}

bool TicketCompareUser(void *a, void *b)
{
  TicketNode *nodeA = (TicketNode *)a;
  TicketNode *nodeB = (TicketNode *)b;

  return nodeA->data.userId == nodeB->data.userId;
}

bool TicketCompareTrain(void *a, void *b)
{
  TicketNode *nodeA = (TicketNode *)a;
  TicketNode *nodeB = (TicketNode *)b;

  return nodeA->data.trainId == nodeB->data.trainId;
}
