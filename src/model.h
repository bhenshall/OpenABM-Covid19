/*
 * model.h
 *
 *  Description: Top level model 'object' c
 *  Created on:  5 Mar 2020
 *      Author:  hinchr
 */

#ifndef MODEL_H_
#define MODEL_H_

/************************************************************************/
/******************************* Includes *******************************/
/************************************************************************/

#include "structure.h"
#include "individual.h"
#include "network.h"
#include "params.h"

/************************************************************************/
/****************************** Structures  *****************************/
/************************************************************************/

struct event_list{
	int type;
	event **events;
	long *n_daily;
	long **n_daily_by_age;
	long *n_daily_current;
	long n_total;
	long *n_total_by_age;
	long n_current;
	double **infectious_curve;
};

struct directory{
	long n_idx;
	int *n_jdx;
	long **val;
};

struct model{
	parameters *params;
	individual *population;
	int time;

	interaction *interactions;
	long interaction_idx;
	int interaction_day_idx;
	long n_interactions;
	long *possible_interactions;
	long n_possible_interactions;
	long n_total_intereactions;

	network *random_network;
	network *household_network;
	network **occupation_network;
	directory *household_directory;
	double mean_interactions[ N_AGE_TYPES ];

	event *events;
	event *next_event;
	event_list *event_lists;

	trace_token *trace_tokens;
	trace_token *next_trace_token;
	long n_trace_tokens_used;
	long n_trace_tokens;

	int **transition_time_distributions;

	long n_quarantine_days;
};

struct event{
	individual *individual;
	int type;
	int time;
	event *next;
	event *last;
};

/************************************************************************/
/******************************  Functions  *****************************/
/************************************************************************/

#define n_current( model, type ) ( model->event_lists[type].n_current )
#define n_total( model, type ) ( model->event_lists[type].n_total )
#define n_total_age( model, type, age ) ( model->event_lists[type].n_total_by_age[age] )
#define n_daily( model, type, day ) ( model->event_lists[type].n_daily_current[day] )

model* new_model(parameters *);
void set_up_population( model* );
void set_up_interactions( model* );
void set_up_events( model* );
void set_up_seed_infection( model* );
void set_up_networks( model* );
void set_up_occupation_network( model*, int );
void set_up_individual_hazard( model* );
void destroy_model( model* );

int one_time_step( model* );
void flu_infections( model* );

event* new_event( model* );
event* add_individual_to_event_list( model*, int, individual*, int );
void set_up_event_list( model*, parameters*, int );
void destroy_event_list( model*, int );
void remove_event_from_event_list( model*, event* );
void update_event_list_counters(  model*, int );
void transition_events( model*, int, void( model*, individual* ), int );

void add_interactions_from_network( model*, network*, int, int, double );
void build_daily_newtork( model* );
void build_random_network( model * );

#endif /* MODEL_H_ */
