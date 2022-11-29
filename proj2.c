/*

	+..........................................+
	|Name: Pedro Guilherme de Noronha Guimaraes|
	|ist_id: 102543							   |
	|Proj_1 - IAED							   |
	|2021/2022								   |
	+..........................................+

 */


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "proj2.h"



/* Global variables */

flight flights[MAX_FLIGHTS];    /* Array of all flights registed */
airport airports[MAX_AIRPORTS]; /* Array of all airports registed */
date today;      /* Declaration of global date, defined in Main() */
int airport_count = 0; /* number of airports registed */
int flight_count = 0;  /* number of flights registed */



/* inList() returns true if integer given is in the array of integers given */
int inList(int n, int size, int vec[6]) {
	int i;
	for (i = 0; i < size; i++) {
		if (vec[i] == n) {
			return 1;
		}
	}
	return 0;
}


/* advance_a_day(date) advances one day from given argument date */
date advance_a_day(date d) {
	date n_date;
	int month30[] = {4, 6, 9, 11,};
	if ((d.day == 31 && d.month != 12) || 
		(inList(d.month, 4, month30) && d.day ==30) || (d.day == 28 && 
		d.month == 2)) {
		n_date.day = 1;
		n_date.month = d.month + 1;
		n_date.year = d.year;
	}
	else if(d.month == 12 && d.day == 31) {
		n_date.day = 1;
		n_date.month = 1;
		n_date.year = d.year + 1;
	}
	else {
		n_date.day = d.day + 1;
		n_date.month = d.month;
		n_date.year = d.year;
	}
	return n_date;
}

/* samedate(date1, date2) returns True if the two arguments type date represent
the same day */
int samedate(date d1, date d2) {
	if (d1.day == d2.day && d1.month == d2.month && d1.year == d2.year) {
		return 1;
	}
	return 0;
}


/* passed_date(date1, date2) returns True if date1 represents a day in the
past relatively to date2 */
int passed_date(date d1, date d2) {
	if (d1.year < d2.year)
		return 1;
	else if (d1.year == d2.year && d1.month < d2.month) 
		return 1; 
	else if (d1.year == d2.year && d1.month == d2.month && d1.day < d2.day)
		return 1;
	return 0;
}


/* passed_time (time1, time2) returns True if time1 represents time in 
the past relatively to time2 */
int passed_time(time t1, time t2) {
	if (t1.hour < t2.hour)
		return 1;
	else if (t1.hour == t2.hour && t1.min < t2.min)
		return 1;
	return 0;
}


/* find_arvl_time (departure_time, flight_duration) returns the time of 
arrival of the flight */
time find_arvl_time(time dep_time, time duration) {
	time arvl_time;
	int temp_hour = dep_time.hour + duration.hour;
	int temp_min = dep_time.min + duration.min;
	temp_hour += temp_min / 60;
	temp_min = temp_min % 60;
	temp_hour = temp_hour % 24;
	arvl_time.hour = temp_hour;
	arvl_time.min = temp_min;

	return arvl_time;
}


/* find_arvl_date(arrival_time, departure_time, departure_date) verifies if
the arrival date of the flight is different from departure date and if so
finds the arrival date by advacing a day from departure date */
date find_arvl_date(time arvl_time, time dep_time, date dep_date) {
	date arvl_date;
	if (arvl_time.hour < dep_time.hour){
		arvl_date = advance_a_day(dep_date);	
	}
	else
		return dep_date;
	return arvl_date;
}


/* over_one_year(date) returns True if date represents a day more than 1 year
in the future from global date and False if not */
int over_one_year(date d) {
	date d1;
	int n_d, n_m, n_y;
	n_d = today.day;
	n_m = today.month;
	n_y = today.year;
	d1.day = n_d;
	d1.month = n_m;
	d1.year = n_y + 1;
	if(!samedate(d, d1) && !passed_date(d, d1))
		return 1;
	return 0;
}


/* verify_airport_id(id) verifies if the input ID is in correct format
(3 upper case letters) */
int verify_airport_id(char id[AIRPORT_ID_LEN]) {
	int i;
	for (i = 0; i < AIRPORT_ID_LEN-1; i++) {
		if (id[i] < 'A' || id[i] > 'Z')
			return 0; 
	}
	return 1;
}


/* verify_repeated_airport(id) verifies if the input ID is already associated
with another airport */
int verify_repeated_airport(char id[AIRPORT_ID_LEN]) {
	int i;
	for (i = 0; i <= airport_count; i++) {
		if(strcmp(id, airports[i].id) == 0)
			return 0;
	}
	return 1;
}


int flight_exits(char flight_id[FLIGHT_CODE_LEN], date flight_date){
	int i;
	for (i = 0; i<flight_count; i++){
		if(!strcmp(flights[i].flight_id, flight_id) && 
			samedate(flight_date, flights[i].flight_date))
			return 1;
	}
	return 0;
}


/* verify_flight_code(flight_code) verifies if input flight code is in correct
format (2 upper case letters and a number between 1-9999 not starting with 0 */
int verify_flight_code(char id[FLIGHT_CODE_LEN]) {
	int i = 2;
	int j = 0;
	int code_number = 0;
	char str_code[5] = "\0";
	while(id[i] != '\0') {
		str_code[j] = id[i];
		i++;
		j++;
	}
	code_number = atoi(str_code);
	if(strlen(id)>7 || id[0] < 'A' || id[0] > 'Z'||id[1] < 'A' || id[1] > 'Z' ||
		code_number < 1 || code_number > 9999 || str_code[0] == '0'){
		return 0;
	} 
	return 1;
}


/* verify_repeated_flight(airport_id, departure_date) verifies if there is
already a flight with the same flight code departing in the same day */
int verify_repeated_flight(char id[AIRPORT_ID_LEN], date dep_date) {
	int i;
	for (i = 0; i <= flight_count; i++) {
		if(strcmp(id, flights[i].flight_id) == 0 && 
			samedate(dep_date, flights[i].flight_date)) { 
			puts("flight already exists");
			return 0;
		}
	}
	return 1;
}


/* create_date(string_date) receives a string representing a date and creates
that date in type struct Date */
date create_date(char str_date[DATE_LEN]) {
	int day, month, year;
	date d;
    sscanf(str_date, "%d-%d-%d", &day, &month, &year);
	d.day = day;
	d.month = month;
	d.year = year;
	return d;
}


/* create_time(string_tine) receives a string representing a time of the day
and creates that time in type struct Time */
time create_time(char str_time[TIME_LEN]) {
	int hour, min;
	time t;
	sscanf(str_time, "%d:%d", &hour, &min);
	t.hour = hour;
	t.min = min;
	return t;
}

/* create_airport(line) creates a new airport with specified ID */
void create_airport(char *line) {
	char id[AIRPORT_ID_LEN];
	char country[COUNTRY_LEN];
	char city[CITY_LEN];
	airport a;
	sscanf(line, "%*c %s %s %[^\n]", id, country, city);
	if (!verify_airport_id(id)){
		puts("invalid airport ID");
	}
	else if (airport_count == MAX_AIRPORTS)
		puts("too many airports");
	else if (!verify_repeated_airport(id))
		puts("duplicate airport");
	else {
		strcpy(a.id, id);
		strcpy(a.country, country);
		strcpy(a.city, city);
		airports[airport_count] = a;
		airport_count++;
		printf("airport %s\n", id);
	}
}


/* verify_flight_data checks all the info of a flight inputed and checks if
they are valid */
int verify_flight_data(char dep_airport[AIRPORT_ID_LEN], 
	char arvl_airport[AIRPORT_ID_LEN], char flight_id[FLIGHT_CODE_LEN], 
	int fl_capacity, time duration_format, date date_format) {
	if (!verify_flight_code(flight_id)){
		puts("invalid flight code");
		return 0;
	}
	else if(!verify_repeated_flight(flight_id, date_format)) 
		return 0;
	else if(verify_repeated_airport(dep_airport)) {
		printf("%s: no such airport ID\n", dep_airport);
		return 0;
	}
	else if(verify_repeated_airport(arvl_airport)) {
		printf("%s: no such airport ID\n", arvl_airport);
		return 0;
	}
	else if (flight_count == MAX_FLIGHTS) {
		puts("too many flights");
		return 0;
	}
	else if (passed_date(date_format, today) || over_one_year(date_format)) {
		puts("invalid date");
		return 0;
	}
	else if(duration_format.hour >= 12 && duration_format.min > 0) {
		puts("invalid duration");
		return 0;
	}
	else if(fl_capacity < 10) {
		puts("invalid capacity");
		return 0;
	}
	return 1;
}


/* store_flight creates a new flight and stores it in global array flights*/
void store_flight(char flight_id[FLIGHT_CODE_LEN], 
	char dep_airport[AIRPORT_ID_LEN], char arvl_airport[AIRPORT_ID_LEN], 
	date date_format, time dep_time_format, time duration_format, 
	int fl_capacity) {

	flight fl;

	strcpy(fl.flight_id, flight_id);
	strcpy(fl.dep_airport, dep_airport);
	strcpy(fl.arvl_airport, arvl_airport);

	fl.flight_date = date_format;
	fl.dep_time = dep_time_format;
	fl.duration = duration_format;
	fl.fl_capacity = fl_capacity;

	flights[flight_count] = fl;
	flight_count++;
}


/* create_flight(line) reads flight specs from line and creates flight if they
are valid, if else prints the error */
void create_flight(char *line)
{
	char flight_id[FLIGHT_CODE_LEN], dep_airport[AIRPORT_ID_LEN], 
	arvl_airport[AIRPORT_ID_LEN], dep_date[DATE_LEN], 
	dep_time[TIME_LEN], duration[TIME_LEN];
	int fl_capacity;
	date date_format;
	time dep_time_format;
	time duration_format;

	sscanf(line, "%*c %s %s %s %s %s %s %d", flight_id, dep_airport, 
		arvl_airport, dep_date, dep_time, duration, &fl_capacity);

	date_format = create_date(dep_date);
	dep_time_format = create_time(dep_time);
	duration_format = create_time(duration);

	if (verify_flight_data(dep_airport, arvl_airport, flight_id, fl_capacity, 
		duration_format, date_format)) {
		store_flight(flight_id, dep_airport, arvl_airport, date_format, 
			dep_time_format, duration_format, fl_capacity);
	}
}


/* compare_flight(flight1, flight2, departure_arrival_switch) compares the
departure dates or the arrival dates depending on the switch, and returns 0
if flight1 departs or arrives before flight2 */
int compare_flight(flight f1, flight f2, int dep) {
	date d1, d2;
	time t1, t2;
	if (dep) {
		t1 = f1.dep_time;
		t2 = f2.dep_time;
		d1 = f1.flight_date;
		d2 = f2.flight_date;
	}
	else {
		t1 = find_arvl_time(f1.dep_time, f1.duration);
		t2 = find_arvl_time(f2.dep_time, f2.duration);
		d1 = find_arvl_date(t1, f1.dep_time, f1.flight_date);
		d2 = find_arvl_date(t2, f2.dep_time, f2.flight_date); 
	}
	if (samedate(d1, d2)) {
		if (passed_time(t1, t2)) 
			return 0;
		return 1;
	}
	else if (passed_date(d1, d2)) 
		return 0;
	return 1;
}


/* get_n_flights(a) returns the number of flights that depart from airport a */
int get_n_flights(airport a) {
	int i;
	int count = 0;
	for (i = 0; i < flight_count; i++) {
		if (strcmp(flights[i].dep_airport, a.id) == 0) {
			count += 1;
		}
	}
	return count;
}


/* compare_airport_id(airport1, airport2) returns 1 if airport1 ID is
alphabetically inferior to airport2 ID */
int compare_airport_id(airport a1, airport a2) {
	char id1[AIRPORT_ID_LEN];
	char id2[AIRPORT_ID_LEN];

	strcpy(id1, a1.id);
	strcpy(id2, a2.id);

	if(id1[0] < id2[0])
		return 1;
	else if(id1[0] == id2[0] && id1[1] < id2[1])
		return 1;
	else if(id1[0] == id2[0] && id1[1] == id2[1] && id1[2] < id2[2])
		return 1;
	return 0;
}


/* swap_airport(*a1, *a2) swaps the two airports */
void swap_airport(airport *xp, airport *yp) {
    airport temp = *xp;
    *xp = *yp;
    *yp = temp;
}


/* swap_flight(*f1, *f2) swaps the two flights */
void swap_flight(flight *xp, flight *yp) {
    flight temp = *xp;
    *xp = *yp;
    *yp = temp;
}


/* sortFlights(flights, size, departure_arrival_switch) sorts flights by
arrival date or departure date depending on the departure_arrival_switch using
BubbleSort */
void sortFlights(flight arr[], int n, int dep)
{
   int i, j;
   for (i = 0; i < n-1; i++)      
       for (j = 0; j < n-i-1; j++)
           if (compare_flight(arr[j], arr[j+1], dep))
              swap_flight(&arr[j], &arr[j+1]);
}


/* sortAirports(airports, size) sorts airports in alphabetical order of their
IDs using BubbleSort */
void sortAirports(airport arr[], int n) {
	int i, j;
	   for (i = 0; i < n-1; i++)      
       for (j = 0; j < n-i-1; j++)
           if (!compare_airport_id(arr[j], arr[j+1]))
              swap_airport(&arr[j], &arr[j+1]);
}


/* find_airport(airportID) finds airport with specified ID in the global
list airports that stores all the airports */
airport find_airport(char id[AIRPORT_ID_LEN]) {
	int i;
	airport a;
	for (i = 0; i < airport_count; i++) {
		if (strcmp(id, airports[i].id) == 0) {
			a = airports[i];
		}
	}
	return a;
}


/* list_airports_IDs(line) reads IDs from line and prints airports with the 
same ID */
void list_airports_IDs(char *line) {
	airport a;
	char ID[AIRPORT_ID_LEN];
	char remainder[MAX_LINE_LEN];

	sscanf(line, "%*c %[^\n]", remainder);

	while(sscanf(remainder, "%s %[^\n]", ID, remainder)==2) {
		if (verify_repeated_airport(ID)) {
			printf("%s: no such airport ID\n", ID);
		}
		else {
			a = find_airport(ID);
			printf("%s %s %s %d\n", a.id, a.city, a.country, get_n_flights(a));
		}
	}
	if (verify_repeated_airport(ID)) {
			printf("%s: no such airport ID\n", ID);
		}
		else {
			a = find_airport(ID);
			printf("%s %s %s %d\n", a.id, a.city, a.country, get_n_flights(a));
	}
}


/* list_airports(line) prints all airports in alphabetical order if input line
contains only the command l, if else calls function list_airports_IDs(line)
that prints the flights with the inputed ID */
void list_airports(char *line) {
	int i;
	if (strlen(line) == 1) { 
		sortAirports(airports, airport_count);
		for (i = 0; i < airport_count; i++) {
			printf("%s %s %s %d\n", airports[i].id,
				airports[i].city,
				airports[i].country,
				get_n_flights(airports[i]));
		}
	}
	else
		list_airports_IDs(line);
}



/* list_flights() prints all the flights stored in global array flights
in the order they were added */
void list_flights()
{
	int i;
	for (i = 0; i < flight_count; i++) {
		printf("%s %s %s %02d-%02d-%d %02d:%02d\n", flights[i].flight_id, 
			flights[i].dep_airport, flights[i].arvl_airport, 
			flights[i].flight_date.day, flights[i].flight_date.month,
			flights[i].flight_date.year, flights[i].dep_time.hour,
			flights[i].dep_time.min); 
	}
}


/* create_list_flights(line) will print all flights in memory or create a 
new flight if string line contains more than just the command */
void create_list_flights(char *line){
	if (strlen(line) == 1)
		list_flights();
	else                           
		create_flight(line);
}


void list_flights_departures(char *line)
{
	int i;
	int j = 0;
	char airport_id[AIRPORT_ID_LEN];
	flight sortedFlights[MAX_FLIGHTS];
	sscanf(line, "%*c %s", airport_id);
	if(verify_repeated_airport(airport_id))
		printf("%s: no such airport ID\n", airport_id);
	else {
		for (i = 0; i < flight_count; i++) {
			if (strcmp(flights[i].dep_airport, airport_id) == 0) {
				/* adding flights to new list sortedFlights*/
				sortedFlights[j] = flights[i];
				j++;
			}
		}
		if (j) {
			sortFlights(sortedFlights, j, 1); /* Bubble sorting the new list */
			for (i = 0; i < j; i++) {
				printf("%s %s %02d-%02d-%d %02d:%02d\n", 
					sortedFlights[i].flight_id,
					sortedFlights[i].arvl_airport, 
					sortedFlights[i].flight_date.day, 
					sortedFlights[i].flight_date.month, 
					sortedFlights[i].flight_date.year, 
					sortedFlights[i].dep_time.hour,
					sortedFlights[i].dep_time.min);
			}
		}
	}
}


/* list_flights_arrivals(line) prints all the flights that arrive at 
a certain airport (arvl airport id in argument line) in ascending
chronological order */
void list_flights_arrivals(char *line)
{
	int i;
	/* int j will count the number of flights that arrive at pretended airport*/
	int j = 0;
	char airport_id[AIRPORT_ID_LEN];
	time arvl_time;
	date arvl_date;
	flight sortedFlights[MAX_FLIGHTS];

	sscanf(line, "%*c %s", airport_id);

	if(verify_repeated_airport(airport_id))
		printf("%s: no such airport ID\n", airport_id);
	else {
		for (i = 0; i < flight_count; i++) {
			if (strcmp(flights[i].arvl_airport, airport_id) == 0) {
				/* adds flights to new list sortedFlights */
				sortedFlights[j] = flights[i];
				j++;
			}
		}

		if (j) {
			sortFlights(sortedFlights, j, 0); /* Bubble sorting the new list */
			for (i = 0; i < j; i++) {
				arvl_time = find_arvl_time(sortedFlights[i].dep_time, 
					sortedFlights[i].duration);
				arvl_date = find_arvl_date(arvl_time, sortedFlights[i].dep_time,
					sortedFlights[i].flight_date);
				printf("%s %s %02d-%02d-%d %02d:%02d\n", 
					sortedFlights[i].flight_id,
					sortedFlights[i].dep_airport,
					arvl_date.day, 
					arvl_date.month, 
					arvl_date.year, 
					arvl_time.hour,
					arvl_time.min);
			}
		}
	}
}


/* datetime_foward(line) advances the global date to the date inputed */
void datetime_foward(char *line)
{
	char n_date_str[DATE_LEN];
	date n_date;

	sscanf(line, "%*c %s", n_date_str);
	n_date = create_date(n_date_str);

	if (passed_date(n_date, today) || over_one_year(n_date))
		puts("invalid date");
	else {
		today.day = n_date.day;
		today.month = n_date.month;
		today.year = n_date.year;
		printf("%02d-%02d-%d\n", n_date.day, n_date.month, n_date.year);
	}
}


void print_linked_reserv(hash_entry *entry, char flight_id[FLIGHT_CODE_LEN], 
	date Date){
	hash_entry *temp;
	while(entry != NULL){
		if(!strcmp(entry->reserv->flight_id, flight_id) && 
			samedate(entry->reserv->flight_date, Date)){
			printf("%s %d\n", entry->reserv->reservation_code, 
				entry->reserv->n_passengers);
		}
		temp=entry;
		entry=temp->next;
	}

}

void print_linked(hash_entry *entry){
	hash_entry *temp;
	while(entry != NULL){
		printf("%s %d\n", entry->reserv->reservation_code, 
				entry->reserv->n_passengers);
		temp = entry;
		entry = temp->next;
	}
}


void list_reservations(hash_table *hasht, char flight_id[FLIGHT_CODE_LEN], 
	date Date){
	hash_entry *reserv = hasht_get(hasht, flight_id);
	if(reserv!=NULL){
		hash_entry *n_head;
		n_head = quick_sort_linked(&reserv, hasht, reserv->reserv->flight_id);
		print_linked_reserv(n_head, flight_id, Date);
	}
}


int invalid_res_code(char *reserv_code){
	unsigned long i = 0;
	if(strlen(reserv_code)<10)
		return 1;
	while(reserv_code[i]){
		if(!(reserv_code[i]>='A' && reserv_code[i]<='Z') && 
			!(reserv_code[i]>='0' && reserv_code[i]<='9')){
			return 1;
		}
		i++;
	}
	return 0;
}

/*
int reserv_code_used(char *reserv_code, hash_table *resvcode_hash){
	hash_entry *head = hash_resvcode_get(resvcode_hash, reserv_code); 	
	while(head != NULL){
		if(!strcmp(head->reserv->reservation_code, reserv_code)){
			free(head);
			return 1;
		}
		head = head->next;
	}
	free(head);
	return 0;
}
*/

int reserv_code_used(char *reserv_code, hash_table *resvcode_hash){
	hash_entry *head = hash_resvcode_get(resvcode_hash, reserv_code);	
	if(head != NULL)
		return 1;
	return 0;
}


int get_capacity(char flight_id[FLIGHT_CODE_LEN], date date){
	int i;
	for(i = 0; i < flight_count; i++){
		if(!strcmp(flight_id, flights[i].flight_id) && samedate(flights[i].flight_date, date))
			return flights[i].fl_capacity;
	}
	return 0;
}


int get_total_p(hash_table *hasht, char flight_id[FLIGHT_CODE_LEN], date date){
	int n_passengers = 0;
	hash_entry *entry = hasht_get(hasht, flight_id);
	while(entry != NULL){
		if(!strcmp(flight_id, entry->reserv->flight_id) && 
			samedate(date, entry->reserv->flight_date)){
			n_passengers += entry->reserv->n_passengers;
		}
		entry=entry->next;
	}
	return n_passengers;
}


int over_capacity(reservation *reserv, hash_table *hasht){
	int total_passengers = get_total_p(hasht, reserv->flight_id, reserv->flight_date);
	int capacity = get_capacity(reserv->flight_id, reserv->flight_date);
	if (total_passengers+reserv->n_passengers > capacity){
		return 1;
	}
	return 0;
}

int verify_reservation_data(reservation *reserv, hash_table *hasht,
	hash_table *resvcode_hash){
	if(invalid_res_code(reserv->reservation_code)){
		printf("invalid reservation code\n");
		return 0;
	}
	else if(!flight_exits(reserv->flight_id, reserv->flight_date)){
		printf("%s: flight does not exist\n", reserv->flight_id);
		return 0;
	}
	else if(reserv_code_used(reserv->reservation_code, resvcode_hash)){
		printf("%s: flight reservation already used\n", reserv->reservation_code);
		return 0;
	}
	else if(over_capacity(reserv, hasht)){
		printf("too many reservations\n");
		return 0;
	}
	else if(passed_date(reserv->flight_date, today) || 
		over_one_year(reserv->flight_date)){
		printf("invalid date\n");
		return 0;
	}
	else if(reserv->n_passengers <= 0){
		printf("invalid passenger number\n");
		return 0;
	}
	return 1;
}


int verify_search_resv(char flight_id[FLIGHT_CODE_LEN], date date){
	if(!flight_exits(flight_id, date)){
		printf("%s: flight does not exist\n", flight_id);
		return 0;
	}
	else if(passed_date(date, today) || 
		over_one_year(date)){
		printf("invalid date\n");
		return 0;
	}
	return 1;
}


void add_list_reservations(char *line, hash_table *hasht, 
	hash_table* hash_by_resvcode){
	reservation *res = malloc(sizeof(reservation)); 	
	char flight_code[FLIGHT_CODE_LEN];
	char str_date[DATE_LEN];

	res->reservation_code = malloc(sizeof(char)*strlen(line));

	if(strlen(line) > RES_LINE_LEN){
		sscanf(line, "%*c %s %s %s %d", res->flight_id, str_date, 
			res->reservation_code, &res->n_passengers);
		res->flight_date = create_date(str_date);
		if(verify_reservation_data(res, hasht, hash_by_resvcode)){
			hasht_insert(hasht, res, 1);
			hasht_insert(hash_by_resvcode, res, 0);
		}
		else{
			free(res->reservation_code);
			free(res);
		}
		/*
		exp = hasht_get(hasht, res->flight_id);
		printf("%s", exp->flight_id);
		*/
	}
	else{
		sscanf(line, "%*c %s %s", flight_code, str_date);
		if(verify_search_resv(flight_code, create_date(str_date))){
			list_reservations(hasht, flight_code, create_date(str_date));
		}
	}
}


void del_f(int pos){
	int i;
	for(i=pos; i<flight_count-1; i++)
		flights[i]=flights[i+1];
	flight_count--;
}


void del_flight(char *flight_id){
	int i = 0;
	while(i<flight_count){
		if(!strcmp(flights[i].flight_id, flight_id)){
			del_f(i);
			i--;
		}
		i++;
	}
}


int flight_id_exits(char *code){
	int i;
	for(i=0; i<flight_count; i++){
		if(!strcmp(flights[i].flight_id, code))
			return 1;
	}
	return 0;
}


void del_by_flight(char *code, hash_table *hasht, hash_table *hash_by_resvcode){
	hash_entry *head = hasht_get(hasht, code);
	hash_entry *temp = head;
	hash_entry *prev = temp;
	while(temp != NULL && !strcmp(temp->reserv->flight_id, code)){
		del_revscode(hash_by_resvcode, temp->reserv->reservation_code, NULL);
		head = temp->next;
		free(temp);
	    temp = head;
	}
	while(temp != NULL){
		while(temp != NULL && strcmp(temp->reserv->flight_id, code)){
			prev = temp;
			temp = temp->next;
		}
		if(temp == NULL)
			return;
		prev->next = temp->next;
		free(temp);
		temp = prev->next;
	}
}


int exists_fl_id(char *code){
	int i;
	for(i=0; i<flight_count; i++){
		if(!strcmp(flights[i].flight_id, code))
			return 1;
	}
	return 0;
}


void del_flights_reservations(char *line, hash_table *hasht,
	hash_table *hash_by_resvcode){
	char *code = malloc(sizeof(char)*strlen(line));
	sscanf(line, "%*c %s", code);
	if(strlen(code) <8){
		if(exists_fl_id(code)){
		/*
		del_by_flight(code, hasht, hash_by_resvcode);
		*/
			del_hasht(hasht, code, hash_by_resvcode);
			del_flight(code);
		}
		else
			puts("not found");
	}
	else if(!invalid_res_code(code)){
		if(!reserv_code_used(code, hash_by_resvcode)){
			puts("not found");
		}
		else{
			del_revscode(hash_by_resvcode, code, hasht);
		}
	}
	else{
		puts("not found");
	}
	free(code);
}


/* Exe_commands(command, line) receives line as an argument and calls the
respective function that executes the command */ 
void exe_cmnd(char cmnd[1], char *line, hash_table *hasht, 
	hash_table *hash_by_resvcode) {
	switch(cmnd[0]) {
		case 'q':
			del_hashtables(hasht, hash_by_resvcode);
			free(line);
			exit(0);
			break;
		case 'a': 
			create_airport(line);
			break;
		case 'l':
			list_airports(line);
			break;
		case 'v':
			create_list_flights(line);
			break;
		case 'p':
			list_flights_departures(line);
			break;
		case 'c':
			list_flights_arrivals(line);
			break;
		case 't':
			datetime_foward(line);
			break;
		case 'r':
			add_list_reservations(line, hasht, hash_by_resvcode);
			break;	
		case 'e':
			del_flights_reservations(line, hasht, hash_by_resvcode);
			break;	
	}
}


/* Read input commands from terminal to a string */
void read_comnds(hash_table *hasht, hash_table *hash_by_resvcode) {
	char cmnd[1];
	char *line = malloc(sizeof(char) * MAX_LINE_LEN);

	/* Reading all lines of input until EOF */
	scanf("%[^\n]", line);
	while(line[0] != EOF) {
		sscanf(line, "%c %*[^\n]", cmnd);
		exe_cmnd(cmnd, line, hasht, hash_by_resvcode);
		getchar(); /* remove \n */
		scanf("%[^\n]", line);
	}
	free(line);
}

 
int main() {

	/* Initializa both hash_tables (one with fligh_id as key and the other
	with reservation code as key) */
	hash_table *hasht = create_hasht();
	hash_table *hash_by_resvcode = create_hasht();

	/* Initialize global date as 01-01-2022 */
	today.day = 1;
	today.month = 1;
	today.year = 2022;
	read_comnds(hasht, hash_by_resvcode);

	return 0;
}


