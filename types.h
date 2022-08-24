#ifndef TYPES_H
#define TYPES_H

// User defined types .
typedef unsigned int uint;

// Status will be used in function return type.
typedef enum
{
    success,
    failure
} Status;

//OperationType used in return type of Check operation type function.
typedef enum
{
    help,
    view,
    edit,
    unsupported
} OperationType;

#endif
