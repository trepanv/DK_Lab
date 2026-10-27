
/*
 * Simulation_Run of the ALOHA Protocol
 * 
 * Copyright (C) 2014 Terence D. Todd Hamilton, Ontario, CANADA
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

/*******************************************************************************/

#include <stdio.h>
#include "simparameters.h"
#include "main.h"
#include "output.h"

/*******************************************************************************/

void output_blip_to_screen(Simulation_Run_Ptr simulation_run){
  static int last_percent = -1;
  double percentagedone;
  Simulation_Run_Data_Ptr data;

  data = (Simulation_Run_Data_Ptr) simulation_run_data(simulation_run);
  data->blip_counter++;

  if((data->blip_counter >= BLIPRATE)||(data->number_of_packets_processed >= RUNLENGTH)){
    data->blip_counter = 0;
    percentagedone  =100*(double) data->number_of_packets_processed/RUNLENGTH;
    int current_percent = (int)percentagedone;

    //Only print every 5%
    if (current_percent / 5 == last_percent / 5 &&
        data->number_of_packets_processed < RUNLENGTH) {
      return;
    }
    last_percent = current_percent;

    fprintf(stderr, "%3.0f%% ", percentagedone);
    fprintf(stderr, "Successfully Xmtted Pkts  = %ld (Arrived Pkts = %ld) \r", 
	   data->number_of_packets_processed, 
	   data->arrival_count);
    fflush(stderr);
  }
}


/**********************************************************************/

void output_results(Simulation_Run_Ptr this_simulation_run){
  int i;
  double xmtted_fraction;
  Simulation_Run_Data_Ptr sim_data;
  static int header_printed = 0;
  sim_data = (Simulation_Run_Data_Ptr) simulation_run_data(this_simulation_run);

  if(!header_printed){
    printf("seed,arrival_rate,arrivals,processed,mean_delay,collisions_per_packet\n");
    header_printed=1;
  }

  xmtted_fraction=(double) sim_data->number_of_packets_processed /
    sim_data->arrival_count;

  printf("%u,%.6f,%ld,%ld,%.6f,%.6f\n",
	 sim_data->random_seed, sim_data->packet_arrival_rate,
	 sim_data->arrival_count, sim_data->number_of_packets_processed,
	 sim_data->accumulated_delay / sim_data->number_of_packets_processed,
	 (double) sim_data->number_of_collisions / sim_data->number_of_packets_processed);

  for(i=0; i<NUMBER_OF_STATIONS; i++){
    printf("station,%u,%.6f,%d,%f\n",
	   sim_data->random_seed, sim_data->packet_arrival_rate,
	   i, (sim_data->stations+i)->accumulated_delay/(sim_data->stations+i)->packet_count);
  }
  fflush(stdout);
}



