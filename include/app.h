
#pragma once

#include "context.h"
#include "list.h"
#include "utils.h"

#include "user.h"
#include "train.h"
#include "ticket.h"

typedef struct
{
  List *users, *trains, *tickets;
  User *currentUser;
} AppContext;

bool createAccountActivity(AppContext *context)
{
  clearScreen();
  printf("[ CREATE AN ACCOUNT ]\n\n");
  List *users = context->users;

  User newUser;
  char password[SIZE], confirmPassword[SIZE];
  bool accountCreated;

  printf("Enter name: ");
  getString(newUser.name, 0);

  printf("Enter email: ");
  getString(newUser.email, 0);

  printf("Enter phone number: ");
  getString(newUser.phone, 0);

  while (true)
  {
    printf("Enter password: ");
    getString(password, 0);

    printf("Confirm password: ");
    getString(confirmPassword, 0);

    if (equals(password, confirmPassword))
    {
      accountCreated = true;
      break;
    }
    else
    {
      printf("Passwords do not match! Do you want to retry?: ");

      char choice[16];
      getString(choice, 0);

      if (!equalsIgnoreCase(choice, "y"))
      {
        accountCreated = false;
        break;
      }
    }
  }

  if (accountCreated)
  {
    set(newUser.password, password);
    listInsert(users, &newUser);

    printf("User created successfully!\n");

    return true;
  }
  else
  {
    printf("Account not created!\n");
  }

  return false;
}

User *loginActivity(AppContext *context)
{
  List *users = context->users;
  User *currentUser = NULL;
  User user;
  char choice[16];

  while (true)
  {
    clearScreen();

    printf("[ LOGIN ]\n\n");

    printf("Enter phone: ");
    getString(user.phone, 0);

    printf("Enter password: ");
    getString(user.password, 0);

    UserCondition condition = {
        .data = user,
        .compare = UserComparePhone};

    currentUser = (User *)listFind(users, &condition);

    if (currentUser)
    {
      printf("Logged in successfully!\n");
      waitForKey();

      context->currentUser = currentUser;
      return currentUser;
    }
    else
    {
      printf("Could not log in. Would you like to try again?(y/n): ");
      getString(choice, 0);

      if (!equalsIgnoreCase(choice, "y"))
      {
        return NULL;
      }
    }
  }

  return NULL;
}

CALLBACK int displayAvailableTrains(int index, void *node, void *data)
{
  Train *train = TrainGetData(node);

  printf("  %d. %s | Code: %s\n", index + 1, train->name, train->code);
  return 0;
}

ReturnCode bookTicket(AppContext *context)
{
  while (true)
  {
    clearScreen();

    printf("[ BOOK A TICKET ]\n\n");

    printf("List of available trains:\n");
    listIterate(context->trains, displayAvailableTrains, NULL);

    char code[SIZE];
    Train *train = NULL;

    printf("\nEnter train code: \n");
    getString(code, 0);

    TrainCondition condition = {
        .compare = TrainCompareCode};
    set(condition.data.code, code);

    train = (Train *)listFind(context->trains, &condition);
    if (train)
    {
      Ticket ticket = {
          .userId = context->currentUser->id,
          .trainId = train->id,
          .quantity = 1,
          .paid = 0,
          .total = 1000,
      };

      listInsert(context->tickets, &ticket);

      printf("\n[ Ticket booked successfully! ]\n");

      waitForKey();
      return SUCCESS;
    }
    else
    {
      printf("Invalid code entered! Would you like to try again?(y/n): ");
      char choice[16];
      getString(choice, 0);

      if (!equalsIgnoreCase(choice, "y"))
      {
        return FAILURE;
      }
    }
  }

  return SUCCESS;
}

CALLBACK int displayBookedTickets(int index, void *node, void *data)
{
  AppContext *context = (AppContext *)data;
  Ticket *ticket = TicketGetData(node);

  if (ticket->userId == context->currentUser->id)
  {
    TrainCondition condition = {
        .data = {.id = ticket->trainId},
        .compare = TrainCompareId};

    Train *train = (Train *)listFind(context->trains, &condition);
    int difference = ticket->total - ticket->paid;

    printf("  %d. %s %s\n"
           "    Quantity: %d\n"
           "    Paid: %d\n"
           "    Remaining: %d\n"
           "    Total: %d\n",
           index + 1, train->code, train->name,
           ticket->quantity,
           ticket->paid,
           difference,
           ticket->total);
  }

  return 0;
}

ReturnCode listBookedTickets(AppContext *context)
{
  clearScreen();

  printf("[ BOOKED TICKETS ]\n\n");
  listIterate(context->tickets, displayBookedTickets, context);

  waitForKey();

  return SUCCESS;
}

typedef struct
{
  AppContext *context;
  int count, ticketNumber;
  Ticket *ticket;
} TicketIndex;

CALLBACK int indexBasedTicketFinder(int index, void *node, void *data)
{
  TicketIndex *idx = (TicketIndex *)data;
  Ticket *ticket = TicketGetData(node);

  if (ticket->userId == idx->context->currentUser->id)
  {
    ++idx->count;

    if (idx->count == idx->ticketNumber)
    {
      idx->ticket = ticket;
      return EXIT;
    }
  }

  return 0;
}

ReturnCode cancelTicket(AppContext *context)
{
  char choice[16];

  while (true)
  {
    clearScreen();

    printf("[ CANCEL TICKET ]\n\n");

    printf("List of booked tickets:\n");
    listIterate(context->tickets, displayBookedTickets, context);

    printf("Enter the ticket number you want to delete: ");
    int ticketNumber = getInt();

    TicketIndex ctx = {
        .context = context,
        .count = 0,
        .ticketNumber = ticketNumber,
        .ticket = NULL};

    listIterate(context->tickets, indexBasedTicketFinder, &ctx);

    /* If ticket is found */
    if (ctx.ticket)
    {
      TicketCondition condition = {
          .data = {.id = ctx.ticket->id},
          .compare = TicketCompareId};

      bool isDeleted = listDelete(context->tickets, &condition);

      if (isDeleted)
      {
        printf("\nBooking successfully cancelled!\n");
      }
      else
      {
        printf("\nCould not cancel booking.\n");
      }

      waitForKey();
      return SUCCESS;
    }
    else
    {
      printf("Invalid input entered. Would you like to try again?(y/n): ");
      getString(choice, 0);

      if (!equalsIgnoreCase(choice, "y"))
      {
        return FAILURE;
      }
    }

    waitForKey();
  }

  return SUCCESS;
}

ReturnCode pay(AppContext *context)
{
  char choice[16];
  while (true)
  {
    clearScreen();
    printf("List of booked tickets:\n");
    listIterate(context->tickets, displayBookedTickets, context);

    printf("Enter the ticket number you want to pay for: ");
    int ticketNumber = getInt();

    TicketIndex ctx = {
        .context = context,
        .count = 0,
        .ticketNumber = ticketNumber,
        .ticket = NULL};

    listIterate(context->tickets, indexBasedTicketFinder, &ctx);

    /* If ticket is found */
    if (ctx.ticket)
    {
      Ticket *ticket = ctx.ticket;
      int difference = ticket->total - ticket->paid;
      bool shouldCancel = false;

      while (!shouldCancel)
      {
        if (difference > 0)
        {
          printf("Enter the amount you want to pay: ");
          int amount = getInt();

          if (amount <= 0 || amount > difference)
          {
            // Invalid amount
            printf(
                "Enter an amount between %d and %d.\n"
                "Do you want to retry?: ",
                1, difference);

            getString(choice, 0);
            if (equalsIgnoreCase(choice, "y"))
            {
              continue;
            }
            else
            {
              printf("Payment cancelled.\n");
              shouldCancel = true;
            }
          }
          else
          {
            ticket->paid += amount;
            difference = ticket->total - ticket->paid;

            printf(
                "\n\n[ Updated Ticket Info ]\n"
                "Ticket ID: %d\n"
                "Paid: %d\n"
                "Remaining: %d\n"
                "Total: %d\n",
                ticket->id,
                ticket->paid,
                difference,
                ticket->total);

            printf(
                "\nPayment done successfully!\n"
                "Would you like to do more payments?: ");

            getString(choice, 0);
            if (!equalsIgnoreCase(choice, "y"))
            {
              return SUCCESS;
            }

            shouldCancel = true;
          }
        }
        else
        {
          printf("Ticket fee already paid!\n");
        }
      }
    }
    else
    {
      printf("Invalid input entered. Would you like to try again?(y/n): ");
      getString(choice, 0);

      if (!equalsIgnoreCase(choice, "y"))
      {
        return FAILURE;
      }
    }
  }

  return SUCCESS;
}

ReturnCode displayUserDetails(AppContext *context)
{
  clearScreen();

  User *user = context->currentUser;

  printf(
      "[ USER DETAILS ]\n\n"
      "Name: %s\n"
      "Email: %s\n"
      "Phone: %s\n",
      user->name,
      user->email,
      user->phone);

  waitForKey();

  return SUCCESS;
}

ReturnCode logout(AppContext *context)
{
  context->currentUser = NULL;
  return SUCCESS;
}

ReturnCode mainScreen(AppContext *context)
{
  clearScreen();
  int choice;

  printf(
      "[ MENU ]\n"
      "1. Book a ticket\n"
      "2. List booked tickets\n"
      "3. Cancel a ticket\n"
      "4. Pay\n"
      "5. Display details\n"
      "6. Log out\n"
      "7. Exit\n");
  printf("Enter your choice: ");
  choice = getInt();

  switch (choice)
  {
  case 1:
  {
    // Book a ticket
    bookTicket(context);
    break;
  }
  case 2:
  {
    // List booked tickets
    listBookedTickets(context);
    break;
  }
  case 3:
  {
    // Cancel a ticket
    cancelTicket(context);
    break;
  }
  case 4:
  {
    // Pay
    pay(context);
    break;
  }
  case 5:
  {
    // Display user details
    displayUserDetails(context);
    break;
  }
  case 6:
  {
    // Log out
    logout(context);

    break;
  }
  case 7:
  {
    // Exit
    return EXIT;
  }
  default:
  {
    printf("Invalid option chosen!\n");
  }
  }

  return 0;
}

ReturnCode authScreen(AppContext *context)
{
  int choice;

  while (true)
  {
    clearScreen();
    printf(
        "[ MENU ]\n"
        "1. Create an account\n"
        "2. Log in\n"
        "3. Exit\n");

    printf("Enter your choice: ");
    choice = getInt();

    switch (choice)
    {
    case 1:
    {
      createAccountActivity(context);
      break;
    }
    case 2:
    {
      User *res = loginActivity(context);
      if (res)
      {
        return SUCCESS;
      }
      break;
    }
    case 3:
    {
      return EXIT;

      break;
    }
    default:
    {

      printf("Invalid choice entered.\n");
    }
    }
  }
}

int startApp()
{
  List *users = (List *)UserCreateList();
  List *trains = (List *)TrainCreateList();
  List *tickets = (List *)TicketCreateList();

  /* Sample trains */
  Train a = {
      .code = "CODEA",
  };
  set(a.name, "SACHKHAND");

  Train b = {
      .code = "CODEB",
  };
  set(b.name, "BRIHAN");

  Train c = {
      .code = "CODEC",
  };
  set(c.name, "HIMACHAL");

  listInsert(trains, &a);
  listInsert(trains, &b);
  listInsert(trains, &c);

  AppContext context = {
      users,
      trains,
      tickets,
      .currentUser = NULL};

  ReturnCode ret;

  while (true)
  {
    if (!context.currentUser)
    {
      ret = authScreen(&context);
    }
    else
    {
      ret = mainScreen(&context);
    }

    if (ret == EXIT)
    {
      break;
    }
  }

  printf("Exitting...\n");

  listFree(users);
  listFree(trains);
  listFree(tickets);

  return 0;
}
