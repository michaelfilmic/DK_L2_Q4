
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

#include <math.h>
#include <stdio.h>
#include "main.h"
#include "packet_transmission.h"
#include "packet_arrival.h"

/******************************************************************************/

/*
 * This function will schedule a packet arrival at a time given by
 * event_time. At that time the function "packet_arrival" (located in
 * packet_arrival.c) is executed. An object can be attached to the event and
 * can be recovered in packet_arrival.c.
 */

long int
schedule_packet_arrival_event(Simulation_Run_Ptr simulation_run,
			      double event_time)
{
  Event event;

  event.description = "SW1 Packet Arrival";
  event.function = packet_arrival_event;
  event.attachment = (void *) NULL;

  return simulation_run_schedule_event(simulation_run, event, event_time);
}

long int
schedule_packet_arrival_event_sw2(Simulation_Run_Ptr simulation_run,
			      double event_time)
{
  Event event;

  event.description = "SW2 Packet Arrival";
  event.function = packet_arrival_event_sw2;
  event.attachment = (void *) NULL;

  return simulation_run_schedule_event(simulation_run, event, event_time);
}

long int
schedule_packet_arrival_event_sw3(Simulation_Run_Ptr simulation_run,
			      double event_time)
{
  Event event;

  event.description = "SW3 Packet Arrival";
  event.function = packet_arrival_event_sw3;
  event.attachment = (void *) NULL;

  return simulation_run_schedule_event(simulation_run, event, event_time);
}

long int
schedule_packet_arrival_event_sw2_only_once(Simulation_Run_Ptr simulation_run, double event_time, Packet_Ptr packet)
{
  Event event;

  event.description = "from SW1 only once SW2 Packet Arrival";
  event.function = packet_arrival_event_sw2_only_once;
  event.attachment = (Packet_Ptr *) packet;

  return simulation_run_schedule_event(simulation_run, event, event_time);
}

long int
schedule_packet_arrival_event_sw3_only_once(Simulation_Run_Ptr simulation_run, double event_time, Packet_Ptr packet)
{
  Event event;

  event.description = "from SW1 only once SW3 Packet Arrival";
  event.function = packet_arrival_event_sw3_only_once;
  //event.attachment = (void *) NULL;
  event.attachment = (Packet_Ptr *) packet;

  return simulation_run_schedule_event(simulation_run, event, event_time);
}
/******************************************************************************/

/*
 * This is the event function which is executed when a packet arrival event
 * occurs. It creates a new packet object and places it in either the fifo
 * queue if the server is busy. Otherwise it starts the transmission of the
 * packet. It then schedules the next packet arrival event.
 */

void
packet_arrival_event(Simulation_Run_Ptr simulation_run, void * ptr)
{
  Simulation_Run_Data_Ptr data;
  Packet_Ptr new_packet;

  data = (Simulation_Run_Data_Ptr) simulation_run_data(simulation_run);
  data->arrival_count++;

  new_packet = (Packet_Ptr) xmalloc(sizeof(Packet));
  new_packet->arrive_time = simulation_run_get_time(simulation_run);
  new_packet->service_time = get_packet_transmission_time();
  new_packet->status = WAITING;

  /* 
   * Start transmission if the data link is free. Otherwise put the packet into
   * the buffer.
   */

  if(server_state(data->link) == BUSY) {
    fifoqueue_put(data->buffer, (void*) new_packet);
  } else {
    start_transmission_on_link(simulation_run, new_packet, data->link);
  }

  /* 
   * Schedule the next packet arrival. Independent, exponentially distributed
   * interarrival times gives us Poisson process arrivals.
   */

#ifdef D_D_1_system
  schedule_packet_arrival_event(simulation_run,simulation_run_get_time(simulation_run) + (double) 1/data->packet_arrival_rate);
#else
  schedule_packet_arrival_event(simulation_run,simulation_run_get_time(simulation_run) + exponential_generator((double) 1/data->packet_arrival_rate));
#endif
}

void
packet_arrival_event_sw2(Simulation_Run_Ptr simulation_run, void * ptr)
{
  Simulation_Run_Data_Ptr data;
  Packet_Ptr new_packet;

  data = (Simulation_Run_Data_Ptr) simulation_run_data(simulation_run);
  data->arrival_count_2++;

  new_packet = (Packet_Ptr) xmalloc(sizeof(Packet));
  new_packet->source_id = 2;
  new_packet->arrive_time = simulation_run_get_time(simulation_run);
  new_packet->service_time = get_packet_transmission_time_sw2();
  
  //printf("service time  %f\n", get_packet_transmission_time_sw2());

  new_packet->status = WAITING;

  /* 
   * Start transmission if the data link is free. Otherwise put the packet into
   * the buffer.
   */

  if(server_state(data->link_2) == BUSY) {
    fifoqueue_put(data->buffer_2, (void*) new_packet);
  } else {
    start_transmission_on_link_sw2(simulation_run, new_packet, data->link_2);
  }

  /* 
   * Schedule the next packet arrival. Independent, exponentially distributed
   * interarrival times gives us Poisson process arrivals.
   */

#ifdef D_D_1_system
  schedule_packet_arrival_event_sw2(simulation_run,simulation_run_get_time(simulation_run) + (double) 1/data->packet_arrival_rate_2);
#else
  schedule_packet_arrival_event_sw2(simulation_run,simulation_run_get_time(simulation_run) + exponential_generator((double) 1/data->packet_arrival_rate_2));
#endif
}

void
packet_arrival_event_sw3(Simulation_Run_Ptr simulation_run, void * ptr)
{
  Simulation_Run_Data_Ptr data;
  Packet_Ptr new_packet;

  data = (Simulation_Run_Data_Ptr) simulation_run_data(simulation_run);
  data->arrival_count_3++;

  new_packet = (Packet_Ptr) xmalloc(sizeof(Packet));
  new_packet->source_id = 3;
  new_packet->arrive_time = simulation_run_get_time(simulation_run);
  new_packet->service_time = get_packet_transmission_time_sw3();
  new_packet->status = WAITING;

  /* 
   * Start transmission if the data link is free. Otherwise put the packet into
   * the buffer.
   */

  if(server_state(data->link_3) == BUSY) {
    fifoqueue_put(data->buffer_3, (void*) new_packet);
  } else {
    start_transmission_on_link_sw3(simulation_run, new_packet, data->link_3);
  }

  /* 
   * Schedule the next packet arrival. Independent, exponentially distributed
   * interarrival times gives us Poisson process arrivals.
   */

#ifdef D_D_1_system
  schedule_packet_arrival_event_sw3(simulation_run,simulation_run_get_time(simulation_run) + (double) 1/data->packet_arrival_rate_3);
#else
  schedule_packet_arrival_event_sw3(simulation_run,simulation_run_get_time(simulation_run) + exponential_generator((double) 1/data->packet_arrival_rate_3));
#endif
}

void
packet_arrival_event_sw2_only_once(Simulation_Run_Ptr simulation_run, void * ptr)
{
  Simulation_Run_Data_Ptr data;
  Packet_Ptr sw1_packet;
  sw1_packet = ptr;

  data = (Simulation_Run_Data_Ptr) simulation_run_data(simulation_run);
  //data->arrival_count_2++;
  if (sw1_packet->source_id != 1)
  {
      printf("Error source_id not eq 1 ! \n"); 
  }

  //sw1_packet = (Packet_Ptr) xmalloc(sizeof(Packet));
  //sw1_packet->arrive_time = simulation_run_get_time(simulation_run);
  sw1_packet->source_id = 1;
  sw1_packet->service_time = get_packet_transmission_time_sw2();
  sw1_packet->status = WAITING;

  /* 
   * Start transmission if the data link is free. Otherwise put the packet into
   * the buffer.
   */

  if(server_state(data->link_2) == BUSY) {
    fifoqueue_put(data->buffer_2, (void*) sw1_packet);
  } else {
    start_transmission_on_link_sw2_only_once(simulation_run, sw1_packet, data->link_2);
  }

  /* 
   * Schedule the next packet arrival. Independent, exponentially distributed
   * interarrival times gives us Poisson process arrivals.
   */
}

void
packet_arrival_event_sw3_only_once(Simulation_Run_Ptr simulation_run, void * ptr)
{
  Simulation_Run_Data_Ptr data;
  Packet_Ptr sw1_packet;
  sw1_packet = ptr;

  data = (Simulation_Run_Data_Ptr) simulation_run_data(simulation_run);
  //data->arrival_count_3++;
  if (sw1_packet->source_id != 1)
  {
      printf("Error source_id not eq 1 ! \n"); 
  }

  //sw1_packet = (Packet_Ptr) xmalloc(sizeof(Packet));
  sw1_packet->source_id = 1;
  //sw1_packet->arrive_time = simulation_run_get_time(simulation_run);
  sw1_packet->service_time = get_packet_transmission_time_sw3();
  sw1_packet->status = WAITING;

  /* 
   * Start transmission if the data link is free. Otherwise put the packet into
   * the buffer.
   */

  if(server_state(data->link_3) == BUSY) {
    fifoqueue_put(data->buffer_3, (void*) sw1_packet);
  } else {
    start_transmission_on_link_sw3_only_once(simulation_run, sw1_packet, data->link_3);
  }

  /* 
   * Schedule the next packet arrival. Independent, exponentially distributed
   * interarrival times gives us Poisson process arrivals.
   */
}
