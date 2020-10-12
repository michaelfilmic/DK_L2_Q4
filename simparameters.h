
/*
 * 
 * Simulation of A Single Server Queueing System
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

#ifndef _SIMPARAMETERS_H_
#define _SIMPARAMETERS_H_

/******************************************************************************/

#define FAST_RUN
//#define NO_CSV_OUTPUT
//#define D_D_1_system

#define PACKET_ARRIVAL_RATE 75
#define PACKET_ARRIVAL_RATE_SW2 50
#define PACKET_ARRIVAL_RATE_SW3 50

#ifdef FAST_RUN

#define P12_CUTOFF 0.50

#ifdef D_D_1_system
#define PACKET_LENGTH 10 /* bits */
            //#define RUNLENGTH 10 /* packets */
#define RUNLENGTH 2 /* packets */
#define RANDOM_SEED_LIST 400050636
#else

#define PACKET_LENGTH 1000 /* bits */
            //#define RUNLENGTH 10 /* packets */
#define RUNLENGTH 3 /* packets */
#define RANDOM_SEED_LIST 400050636, 30002

#endif //D_D_1_system

#else

#define P12_CUTOFF 0.23, 0.31 
#define PACKET_LENGTH 1000 /* bits */
#define RUNLENGTH 3000 /* packets */

/* Comma separated list of random seeds to run. */
#define RANDOM_SEED_LIST 400050636, 400099173, 225, 766, 590, 309, 519, 430, 638, 126 

#endif //FAST_RUN

#ifdef D_D_1_system

#define PACKET_XMT_TIME 0.002
#define PACKET_XMT_TIME_SW2 0.003
#define PACKET_XMT_TIME_SW3 0.003
#define BLIPRATE (RUNLENGTH/1000)

#else

#define PACKET_XMT_TIME ((double) PACKET_LENGTH/2E6)//((double) PACKET_LENGTH/LINK_BIT_RATE)
#define PACKET_XMT_TIME_SW2 ((double) PACKET_LENGTH/1E6)//((double) PACKET_LENGTH/LINK_BIT_RATE)
#define PACKET_XMT_TIME_SW3 ((double) PACKET_LENGTH/1E6)//((double) PACKET_LENGTH/LINK_BIT_RATE)
#define BLIPRATE (RUNLENGTH/1000)

#endif //D_D_1_system

/******************************************************************************/

#endif /* simparameters.h */



