#ifndef STUDENT
    #define STUDENT
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "project2.h"
    unsigned int ack = 1;
    unsigned int nack = 0;

    struct message_queue{ 
        unsigned int is_empty;
        struct msg message;
        struct message_queue *next;
    };

    unsigned int A_sequence_num;
    unsigned int A_recent_sequence_num;
    unsigned int A_last_received_sequence_num;
    unsigned int A_ack;
    unsigned int A_received_message;

    unsigned int B_sequence_num;
    unsigned int B_recent_sequence_num;
    unsigned int B_last_received_sequence_num;
    unsigned int B_ack;
    unsigned int B_received_message;

    struct msg *A_next_message;
    struct message_queue *A_message_queue_start; 
    struct message_queue *A_message_queue_end;

    struct msg *B_next_message;
    struct message_queue *B_message_queue_start;
    struct message_queue *B_message_queue_end; 

    unsigned int generate_check_sum(char* vdata, int acknum, int seqnum); 
    unsigned int test_check_sum(struct pkt packet);
    struct msg dequeue(struct message_queue **queue_start);
    void enqueue(struct message_queue **queue_end, struct msg message);
    void send(int AorB, int ack_num, int seq_num, int check_sum, struct msg content);
    void accept(int AorB, struct pkt content);
#endif