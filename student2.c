// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include "project2.h"
#include "student2.h"
 
/* ***************************************************************************
 ALTERNATING BIT AND GO-BACK-N NETWORK EMULATOR: VERSION 1.1  J.F.Kurose

   This code should be used for unidirectional or bidirectional
   data transfer protocols from A to B and B to A.
   Network properties:
   - one way network delay averages five time units (longer if there
     are other messages in the channel for GBN), but can be larger
   - packets can be corrupted (either the header or the data portion)
     or lost, according to user-defined probabilities
   - packets may be delivered out of order.

   Compile as gcc -g project2.c student2.c -o p2
**********************************************************************/



/********* STUDENTS WRITE THE NEXT SEVEN ROUTINES *********/
/* 
 * The routines you will write are detailed below. As noted above, 
 * such procedures in real-life would be part of the operating system, 
 * and would be called by other procedures in the operating system.  
 * All these routines are in layer 4.
 */

/* 
 * A_output(message), where message is a structure of type msg, containing 
 * data to be sent to the B-side. This routine will be called whenever the 
 * upper layer at the sending side (A) has a message to send. It is the job 
 * of your protocol to insure that the data in such a message is delivered 
 * in-order, and correctly, to the receiving side upper layer.
 */

struct msg dequeue(struct message_queue** queue_start,unsigned int ack){
  struct message_queue *queue = *queue_start;
  while(!queue->is_message){queue = queue->next;}
  struct msg next_message = queue->message;
  if(ack){queue = queue->next;};
  return next_message;
}

void enqueue(struct message_queue** queue_end,struct msg message){
  struct message_queue *queue = *queue_end;
  struct message_queue next_queue_end = {
    .is_message = 1,
    .message = message,
    .next = NULL
  };
  queue->next = &next_queue_end;
  queue = queue->next;
}

// generate the check sum 
unsigned int generate_check_sum(char* vdata, int acknum, int seqnum) {
  int i, checksum = 0;
  for(i = 0; i < MESSAGE_LENGTH; i++){
    checksum += (int)(vdata[i]) * i;
  }
  checksum += acknum * 21;
  checksum += seqnum * 22;
  return checksum;
}

// get current sequence number 
int get_sequence_num(int AorB) {
  int sequence_num;
  if (AorB == AEntity){
    sequence_num = A_sequence_num;
    A_sequence_num = !A_sequence_num;
  }else{
    sequence_num = B_sequence_num;
    B_sequence_num = !B_sequence_num; 
  }
  return sequence_num;
}

// send out a package 
void send(int AorB,int ack_num, int seq_num, int check_sum, char *content) {
  struct pkt packet_to_send = {
    .acknum = ack_num,
    .checksum = generate_check_sum(content, ack_num, seq_num),
    .seqnum = seq_num
  };
  strncpy(packet_to_send.payload, content, MESSAGE_LENGTH);
  tolayer3(AorB,packet_to_send);
}

// accept the incoming package and send to layer 5 
void accept(int AorB, char *content) {
  struct msg message_accepted = {}; 
  strncpy(message_accepted.data, content, MESSAGE_LENGTH);
  tolayer5(AorB,message_accepted);
}

void A_output(struct msg message) {
  enqueue(&A_message_queue_end,message); 
  struct msg next_message = dequeue(&A_message_queue_start,A_ack);

  unsigned int check_sum = generate_check_sum(next_message.data, ack, get_sequence_num(AEntity));
  send(AEntity, ack, get_sequence_num(AEntity), check_sum, next_message.data); 
}

/*
 * Just like A_output, but residing on the B side.  USED only when the 
 * implementation is bi-directional.
 */
void B_output(struct msg message)  {

}

/* 
 * A_input(packet), where packet is a structure of type pkt. This routine 
 * will be called whenever a packet sent from the B-side (i.e., as a result 
 * of a tolayer3() being done by a B-side procedure) arrives at the A-side. 
 * packet is the (possibly corrupted) packet sent from the B-side.
 */
void A_input(struct pkt packet) {
  A_ack = packet.acknum;
}

/*
 * A_timerinterrupt()  This routine will be called when A's timer expires 
 * (thus generating a timer interrupt). You'll probably want to use this 
 * routine to control the retransmission of packets. See starttimer() 
 * and stoptimer() in the writeup for how the timer is started and stopped.
 */
void A_timerinterrupt() {
  
}  

/* The following routine will be called once (only) before any other    */
/* entity A routines are called. You can use it to do any initialization */
void A_init() {
  struct message_queue queue = {
    .is_message = 0
  };
  A_message_queue_start = &queue;
  A_message_queue_end = A_message_queue_start;
  A_sequence_num = 0;
  A_ack = 1;
}


/* 
 * Note that with simplex transfer from A-to-B, there is no routine  B_output() 
 */

/*
 * B_input(packet),where packet is a structure of type pkt. This routine 
 * will be called whenever a packet sent from the A-side (i.e., as a result 
 * of a tolayer3() being done by a A-side procedure) arrives at the B-side. 
 * packet is the (possibly corrupted) packet sent from the A-side.
 */
void B_input(struct pkt packet) { 
  int income_check_sum = packet.checksum;
  int income_ack_num = packet.acknum;
  char *income_content = packet.payload;
  int income_sequence_num = packet.seqnum; 
  int real_check_sum = generate_check_sum(income_content, income_ack_num, income_sequence_num);
  if(income_check_sum - real_check_sum == 0){
    accept(BEntity, packet.payload); 
    send(BEntity,ack,0,0,"");
  }else{
    send(BEntity,nack,0,0,"");
  }
}

/*
 * B_timerinterrupt()  This routine will be called when B's timer expires 
 * (thus generating a timer interrupt). You'll probably want to use this 
 * routine to control the retransmission of packets. See starttimer() 
 * and stoptimer() in the writeup for how the timer is started and stopped.
 */
void  B_timerinterrupt() {
}
/* 
 * The following routine will be called once (only) before any other   
 * entity B routines are called. You can use it to do any initialization 
 */
void B_init() {
  struct message_queue queue = {
    .is_message = 0
  };
  B_message_queue_start = &queue;
  B_message_queue_end = A_message_queue_start;
  B_sequence_num = 0;
  B_ack = 1;
}

