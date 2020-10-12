
/*
 * 
 * Simulation_Run of A Single Server Queueing System
 * 
 * Copyright (C) 2014 Terence D. Todd Hamilton, Ontario, CANADA,
 * todd@mcmaster.ca
 * 
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 3 of the License, or (at your option)
 * any later version.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/******************************************************************************/

#include <stdio.h>
#include "trace.h"
#include "main.h"
#include "output.h"
#include "packet_transmission.h"

/******************************************************************************/

/*
 * This function will schedule the end of a packet transmission at a time given
 * by event_time. At that time the function "end_packet_transmission" (defined
 * in packet_transmissionl.c) is executed. A packet object is attached to the
 * event and is recovered in end_packet_transmission.c.
 */

long
schedule_end_packet_transmission_event(Simulation_Run_Ptr simulation_run,
				       double event_time,
				       Server_Ptr link)
{
  Event event;

  //TRACE(printf("MM_debug in schedule_end_packet_transmission_event.\n");)
  event.description = "SW1 Packet Xmt End";
  event.function = end_packet_transmission_event;
  event.attachment = (void *) link;

  return simulation_run_schedule_event(simulation_run, event, event_time);
}

long
schedule_end_packet_transmission_event_sw2(Simulation_Run_Ptr simulation_run,
				       double event_time,
				       Server_Ptr link)
{
  Event event;

  //TRACE(printf("MM_debug in schedule_end_packet_transmission_event.\n");)
  event.description = "SW2 Packet Xmt End";
  event.function = end_packet_transmission_event_sw2;
  event.attachment = (void *) link;

  return simulation_run_schedule_event(simulation_run, event, event_time);
}

long
schedule_end_packet_transmission_event_sw3(Simulation_Run_Ptr simulation_run,
				       double event_time,
				       Server_Ptr link)
{
  Event event;

  //TRACE(printf("MM_debug in schedule_end_packet_transmission_event.\n");)
  event.description = "SW3 Packet Xmt End";
  event.function = end_packet_transmission_event_sw3;
  event.attachment = (void *) link;

  return simulation_run_schedule_event(simulation_run, event, event_time);
}
long
schedule_end_packet_transmission_event_sw2_only_once(Simulation_Run_Ptr simulation_run,
				       double event_time,
				       Server_Ptr link)
{
  Event event;

  //TRACE(printf("MM_debug in schedule_end_packet_transmission_event.\n");)
  event.description = "from SW1 on SW2 Packet Xmt End";
  event.function = end_packet_transmission_event_sw2_only_once;
  event.attachment = (void *) link;

  return simulation_run_schedule_event(simulation_run, event, event_time);
}

long
schedule_end_packet_transmission_event_sw3_only_once(Simulation_Run_Ptr simulation_run,
				       double event_time,
				       Server_Ptr link)
{
  Event event;

  //TRACE(printf("MM_debug in schedule_end_packet_transmission_event.\n");)
  event.description = "from SW1 on SW3 Packet Xmt End";
  event.function = end_packet_transmission_event_sw3_only_once;
  event.attachment = (void *) link;

  return simulation_run_schedule_event(simulation_run, event, event_time);
}
/******************************************************************************/

/*
 * This is the event function which is executed when the end of a packet
 * transmission event occurs. It updates its collected data then checks to see
 * if there are other packets waiting in the fifo queue. If that is the case it
 * starts the transmission of the next packet.
 */

void
end_packet_transmission_event(Simulation_Run_Ptr simulation_run, void * link)
{
  Simulation_Run_Data_Ptr data;
  Packet_Ptr this_packet, next_packet;

  TRACE(printf("SW1 End Of Packet.\n"););

  data = (Simulation_Run_Data_Ptr) simulation_run_data(simulation_run);

  /* 
   * Packet transmission is finished. Take the packet off the data link.
   */

  this_packet = (Packet_Ptr) server_get(link);

  /* Collect statistics. */
  //data->number_of_packets_processed++;
  printf("sim time (msec) = %f \n",simulation_run_get_time(simulation_run)); 
  printf("ariive time (msec) = %f \n", this_packet->arrive_time); 
  printf("each packet_delay (msec) = %f \n",simulation_run_get_time(simulation_run) - this_packet->arrive_time);
  //data->accumulated_delay += simulation_run_get_time(simulation_run) - this_packet->arrive_time;
  this_packet->source_id = 1;

  /* Output activity blip every so often. */
  output_progress_msg_to_screen(simulation_run);

  double rand_p12;
  rand_p12 = uniform_generator();
  TRACE(printf("rand_p12 %f\n", rand_p12);)
  //prob to put into sw2 or sw3
  if (rand_p12 <= data->p12_cutoff) //p12 = 0.23
  {
  schedule_packet_arrival_event_sw2_only_once(simulation_run, simulation_run_get_time(simulation_run), this_packet);
  }
  else if (rand_p12 > data->p12_cutoff) //p13 = 1 - 0.23
  {
  schedule_packet_arrival_event_sw3_only_once(simulation_run, simulation_run_get_time(simulation_run), this_packet);
  }

  /* 
   * See if there is are packets waiting in the buffer. If so, take the next one
   * out and transmit it immediately.
  */

  if(fifoqueue_size(data->buffer) > 0) {
    next_packet = (Packet_Ptr) fifoqueue_get(data->buffer);
    start_transmission_on_link(simulation_run, next_packet, link);
  }
}

void
end_packet_transmission_event_sw2(Simulation_Run_Ptr simulation_run, void * link)
{
  Simulation_Run_Data_Ptr data;
  Packet_Ptr this_packet, next_packet;

  TRACE(printf("SW2 End Of Packet.\n"););

  data = (Simulation_Run_Data_Ptr) simulation_run_data(simulation_run);

  /* 
   * Packet transmission is finished. Take the packet off the data link.
   */

  this_packet = (Packet_Ptr) server_get(link);

  /* Collect statistics. */
  data->number_of_packets_processed_2++;
  data->accumulated_delay_2 += simulation_run_get_time(simulation_run) - 
    this_packet->arrive_time;

  /* Output activity blip every so often. */
  output_progress_msg_to_screen_sw2(simulation_run);

  /* This packet is done ... give the memory back. */
  xfree((void *) this_packet);

  /* 
   * See if there is are packets waiting in the buffer. If so, take the next one
   * out and transmit it immediately.
  */

  if(fifoqueue_size(data->buffer_2) > 0) {
    next_packet = (Packet_Ptr) fifoqueue_get(data->buffer_2);
    if (next_packet->source_id != 1)
    {
    start_transmission_on_link_sw2(simulation_run, next_packet, link);
    }
    else
    {
    start_transmission_on_link_sw2_only_once(simulation_run, next_packet, link);

    }
  }
}

void
end_packet_transmission_event_sw3(Simulation_Run_Ptr simulation_run, void * link)
{
  Simulation_Run_Data_Ptr data;
  Packet_Ptr this_packet, next_packet;

  //TRACE(printf("MM_debug in end_packet_transmission_event.\n");)
  TRACE(printf("SW3 End Of Packet.\n"););

  data = (Simulation_Run_Data_Ptr) simulation_run_data(simulation_run);

  /* 
   * Packet transmission is finished. Take the packet off the data link.
   */

  this_packet = (Packet_Ptr) server_get(link);

  /* Collect statistics. */
  data->number_of_packets_processed_3++;
  data->accumulated_delay_3 += simulation_run_get_time(simulation_run) - 
    this_packet->arrive_time;

  /* Output activity blip every so often. */
  output_progress_msg_to_screen_sw3(simulation_run);

  /* This packet is done ... give the memory back. */
  xfree((void *) this_packet);

  /* 
   * See if there is are packets waiting in the buffer. If so, take the next one
   * out and transmit it immediately.
  */

  if(fifoqueue_size(data->buffer_3) > 0) {
    next_packet = (Packet_Ptr) fifoqueue_get(data->buffer_3);
    if (next_packet->source_id != 1)
    {
    start_transmission_on_link_sw3(simulation_run, next_packet, link);
    }
    else
    {
    start_transmission_on_link_sw3_only_once(simulation_run, next_packet, link);

    }
  }
}
void
end_packet_transmission_event_sw2_only_once(Simulation_Run_Ptr simulation_run, void * link)
{
  Simulation_Run_Data_Ptr data;
  Packet_Ptr this_packet, next_packet;

  TRACE(printf("from SW1 on SW2 End Of Packet.\n"););

  data = (Simulation_Run_Data_Ptr) simulation_run_data(simulation_run);

  /* 
   * Packet transmission is finished. Take the packet off the data link.
   */

  this_packet = (Packet_Ptr) server_get(link);

  /* Collect statistics. */
  data->number_of_packets_processed++;
  data->accumulated_delay += simulation_run_get_time(simulation_run) - 
    this_packet->arrive_time;

  /* Output activity blip every so often. */
  output_progress_msg_to_screen_sw2(simulation_run);

  /* This packet is done ... give the memory back. */
  xfree((void *) this_packet);

  /* 
   * See if there is are packets waiting in the buffer. If so, take the next one
   * out and transmit it immediately.
  */

  if(fifoqueue_size(data->buffer_2) > 0) {
    next_packet = (Packet_Ptr) fifoqueue_get(data->buffer_2);
    if (next_packet->source_id != 1)
    {
    start_transmission_on_link_sw2(simulation_run, next_packet, link);
    }
    else
    {
    start_transmission_on_link_sw2_only_once(simulation_run, next_packet, link);

    }
  }
}

void
end_packet_transmission_event_sw3_only_once(Simulation_Run_Ptr simulation_run, void * link)
{
  Simulation_Run_Data_Ptr data;
  Packet_Ptr this_packet, next_packet;

  //TRACE(printf("MM_debug in end_packet_transmission_event.\n");)
  TRACE(printf("from SW1 on SW3 End Of Packet.\n"););

  data = (Simulation_Run_Data_Ptr) simulation_run_data(simulation_run);

  /* 
   * Packet transmission is finished. Take the packet off the data link.
   */

  this_packet = (Packet_Ptr) server_get(link);

  /* Collect statistics. */
  data->number_of_packets_processed++;
  data->accumulated_delay += simulation_run_get_time(simulation_run) - 
    this_packet->arrive_time;

  /* Output activity blip every so often. */
  output_progress_msg_to_screen_sw3(simulation_run);

  /* This packet is done ... give the memory back. */
  xfree((void *) this_packet);

  /* 
   * See if there is are packets waiting in the buffer. If so, take the next one
   * out and transmit it immediately.
  */

  if(fifoqueue_size(data->buffer_3) > 0) {
    next_packet = (Packet_Ptr) fifoqueue_get(data->buffer_3);
    if (next_packet->source_id != 1)
    {
    start_transmission_on_link_sw3(simulation_run, next_packet, link);
    }
    else
    {
    start_transmission_on_link_sw3_only_once(simulation_run, next_packet, link);

    }
  }
}
/*
 * This function ititiates the transmission of the packet passed to the
 * function. This is done by placing the packet in the server. The packet
 * transmission end event for this packet is then scheduled.
 */

void
start_transmission_on_link(Simulation_Run_Ptr simulation_run, 
			   Packet_Ptr this_packet,
			   Server_Ptr link)
{
  TRACE(printf("SW1 Start Of Packet.\n");)

  server_put(link, (void*) this_packet);
  this_packet->status = XMTTING;

  /* Schedule the end of packet transmission event. */
  schedule_end_packet_transmission_event(simulation_run,
	 simulation_run_get_time(simulation_run) + this_packet->service_time,
	 (void *) link);
}

void
start_transmission_on_link_sw2(Simulation_Run_Ptr simulation_run, 
			   Packet_Ptr this_packet,
			   Server_Ptr link)
{
  TRACE(printf("SW2 Start Of Packet.\n");)

  server_put(link, (void*) this_packet);
  this_packet->status = XMTTING;

  /* Schedule the end of packet transmission event. */
  schedule_end_packet_transmission_event_sw2(simulation_run,
	 simulation_run_get_time(simulation_run) + this_packet->service_time,
	 (void *) link);
}

void
start_transmission_on_link_sw3(Simulation_Run_Ptr simulation_run, 
			   Packet_Ptr this_packet,
			   Server_Ptr link)
{
  TRACE(printf("SW3 Start Of Packet.\n");)

  server_put(link, (void*) this_packet);
  this_packet->status = XMTTING;

  /* Schedule the end of packet transmission event. */
  schedule_end_packet_transmission_event_sw3(simulation_run,
	 simulation_run_get_time(simulation_run) + this_packet->service_time,
	 (void *) link);
}
void
start_transmission_on_link_sw2_only_once(Simulation_Run_Ptr simulation_run, 
			   Packet_Ptr this_packet,
			   Server_Ptr link)
{
  TRACE(printf("from SW1 packet on SW2 Start Of Packet.\n");)

  server_put(link, (void*) this_packet);
  this_packet->status = XMTTING;

  /* Schedule the end of packet transmission event. */
  schedule_end_packet_transmission_event_sw2_only_once(simulation_run,
	 simulation_run_get_time(simulation_run) + this_packet->service_time,
	 (void *) link);
}

void
start_transmission_on_link_sw3_only_once(Simulation_Run_Ptr simulation_run, 
			   Packet_Ptr this_packet,
			   Server_Ptr link)
{
  TRACE(printf("from SW1 packet on SW3 Start Of Packet.\n");)

  server_put(link, (void*) this_packet);
  this_packet->status = XMTTING;

  /* Schedule the end of packet transmission event. */
  schedule_end_packet_transmission_event_sw3_only_once(simulation_run,
	 simulation_run_get_time(simulation_run) + this_packet->service_time,
	 (void *) link);
}
/*
 * Get a packet transmission time. For now it is a fixed value defined in
 * simparameters.h
 */

double get_packet_transmission_time(void)
{
  return ((double) PACKET_XMT_TIME);
}

double get_packet_transmission_time_sw2(void)
{
  return ((double) PACKET_XMT_TIME_SW2);
}

double get_packet_transmission_time_sw3(void)
{
  return ((double) PACKET_XMT_TIME_SW3);
}

