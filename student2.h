#ifndef STUDENT
    #define STUDENT
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "project2.h"
    unsigned int ack = 1;
    unsigned int nack = 0;

    struct message_queue{ 
        unsigned int is_message;
        struct msg message;
        struct message_queue *next;
    };

    unsigned int A_sequence_num;
    unsigned int A_ack;

    unsigned int B_sequence_num;
    unsigned int B_ack;

    struct message_queue* A_message_queue_start; 
    struct message_queue* A_message_queue_end;

    struct message_queue* B_message_queue_start;
    struct message_queue* B_message_queue_end; 

    struct msg dequeue(struct message_queue **queue_start, unsigned int ack);
    void enqueue(struct message_queue **queue_end, struct msg message);
    void send(int AorB, int ack_num, int seq_num, int check_sum, char *content);
    void accept(int AorB, char *content);
#endif