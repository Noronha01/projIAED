#ifndef PROJ2_H
#define PROJ2_H


#define AIRPORT_ID_LEN 4 /* max length of airport ID */
#define COUNTRY_LEN 31 /* max length of airport country */
#define CITY_LEN 51 /* max length of airport city */
#define FLIGHT_CODE_LEN 7 /* max length of flight code */
#define DATE_LEN 11 /* max date lenght (type string not Struct date)*/
#define TIME_LEN 6 /*  max length of time (type string not Struct time)*/
#define MAX_AIRPORTS 40 /* max number of airports registed */
#define MAX_FLIGHTS 30000 /* max number of flights registed */
#define MAX_LINE_LEN 65535 /* max length of input line */
#define RES_LINE_LEN 19
#define HASH_TABLE_SIZE 90000


/* -------------------------------------------------------------- */
/* Structs Date, Time, Airport, Flight, Hash_Entry and Hash_Table */

/* Defining Date */
typedef struct Date {
	int day, month, year;
} date;


/* Defining Time */
typedef struct Time {
	int min, hour;
} time;


/* Defining Airport */
typedef struct Airport {
	char id[FLIGHT_CODE_LEN], country[COUNTRY_LEN], city[CITY_LEN];
} airport;


/* Defining Flight */
typedef struct Flight {
	char flight_id[FLIGHT_CODE_LEN], dep_airport[AIRPORT_ID_LEN],
	 arvl_airport[AIRPORT_ID_LEN];
	int fl_capacity;
	date flight_date;
	time dep_time;
	time duration;
} flight;


/* Defining Reservation */	
typedef struct Reservation {
	char flight_id[FLIGHT_CODE_LEN];
	date flight_date;
	char *reservation_code;
	int n_passengers;
} reservation;


/* Defining Hash_Entry */
typedef struct Hash_Entry {
	reservation *reserv;
	struct Hash_Entry *next;
} hash_entry;


/* Defining Hash_Table */
typedef struct Hash_Table {
	hash_entry **entries;
} hash_table;


/* --------------------------------------------------------*/
/* Declaration of functions defined in file reservations.c */

/* long hash_func(code) - hash function used for the two hashtables */
long hash_func(char *code);


void del_hashtables(hash_table *hasht, hash_table *hash_by_revscode);


/* hash_table create_hasht() returns the hash_table with all entries set 
to NULL */
hash_table *create_hasht();


/* void hash_insert(hash_table hasht) */
void hasht_insert(hash_table *hasht, reservation *reserv, int key_switch);


/* hash_entry hasht_get(hash_table hasht, char reservation_code) */
hash_entry *hasht_get(hash_table *hasht, char *reservation_code);


/* void del(int pos, hash_entry *entry) deletes entry in entry[pos]*/
void del(int pos, hash_entry *entry);


/* hash_revscode_get(hash_table *hasht, char *reservation code) returns the 
entry of hash table with the reservation code given*/
hash_entry *hash_resvcode_get(hash_table *hasht, char *reservation_code);


/* void del_revscode_hasht(hasht_table *hasht, char *flight_id, char 
*reserv_code) deletes the reservation with reserv_code from hash table with 
flight_id keys */
void del_revscode_hasht(hash_table *hasht, char *flight_id, char *reserv_code);


/* void del_revscode(hash_table *hash_by_revscode, char *resvcode, hash_table 
*hasht) deletes the reservation with revscode from hash table with reservation
code keys and uses previous fuction to delete them from hasht at the same time*/
void del_revscode(hash_table *hash_by_revscode, char *resvcode, hash_table 
	*hasht);


/* void del_hasht(*hasht, *flight_id, *hash_by_revscode) deletes all 
reservations for flight with fligh_id from hasht and hash_by_revscode */
void del_hasht(hash_table *hasht, char *flight_id, hash_table *hash_by_revscode);


/* quick_sort_linked(hash_entry *head, hash_table *hasht, char flight_id) - 
sortes lexicographically the linked list of entries that starts with head */
hash_entry *quick_sort_linked(hash_entry** head, hash_table *hasht, char flight_id[FLIGHT_CODE_LEN]);


#endif
