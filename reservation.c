#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


#include "proj2.h"


/*
add_list_reservations(line) {
	int i = 1;
	int j = 0;
	char flight_code[FLIGHT_CODE_LEN];
	char str_date[DATE_LEN];
	reservation res;
	while(i < strlen(line) && line[i] != ' '){
		flight_code[i-1] = line[i];
		i++;
	}
	i++;
	while(i < strlen(line) && line[i] != ' '){
		str_date[j] = line[i];
		j++;
		i++;
	}
	str_date[j] = '\0';
	i++;
	res.flight_date = create_date(str_date);
	while(i < strlen(line) && line[i] != ' '){

	}
}



void add_list_reservations(char line[MAX_LINE_LEN]){
	reservation res;
	char flight_code[FLIGHT_CODE_LEN];
	char str_date[DATE_LEN];
	if(strlen(line) > RES_LINE_LEN){
		sscanf(line, "%*c %s %s %s %d", res.flight_id, str_date, 
			res.reservation_code, res.n_passengers);
		res.flight_date = create_date(str_date);
	}
	else{
		sscanf(line, "%*c %s %s", flight_code, str_date);
		list_reservations(flight_code, str_date);
		printf(" ");
	}
}
*/


reservation *malloc_reserv(reservation *reserv){
	reservation *new_reserv = malloc(sizeof(reservation));
	if(new_reserv == NULL){
		printf("No memory.\n");
		free(new_reserv);
		exit(0);
	}
	strcpy(new_reserv->flight_id, reserv->flight_id);
	new_reserv->flight_date = reserv->flight_date;
	new_reserv->n_passengers = reserv->n_passengers;
	new_reserv->reservation_code = malloc(sizeof(char)*strlen(reserv->reservation_code));
	if(new_reserv->reservation_code == NULL){
		printf("No memory.\n");
		free(reserv);
		exit(0);
	}
	return new_reserv;
}


long hash_func(char *code){
	long h = 0;
	long n;
	while(*code != '\0'){
		h = (h << 4) + *(code++);
		n = h & 0xF0000000L;
		if (n != 0)
			h ^= n >> 24;	
		h &= ~n;
	}
	return h % HASH_TABLE_SIZE;
}


void del_reservs(hash_entry *entry){
	hash_entry *curr = entry;
	hash_entry *next;
	while(curr != NULL){
		next = curr->next;
		free(curr->reserv->reservation_code);
		free(curr->reserv);
		/*
		free(curr);
		*/
		/* hh */
		free(curr);

		curr = next;
	}
	entry = NULL;
}


void del_hashtables(hash_table *hasht, hash_table *hash_by_revscode){
	int i;
	hash_entry *entry1;
	hash_entry *entry2;
	for(i = 0; i<HASH_TABLE_SIZE; i++){
		entry1 = hasht->entries[i];
		entry2 = hash_by_revscode->entries[i];

		if(entry1 != NULL){
			free(entry1);
			/*
			del_reservs(entry1);
			*/
		}
		if(entry2 != NULL){
			del_reservs(entry2);
		}
	}
	free(entry2);
	free(hasht->entries);
	free(hash_by_revscode->entries);
	free(hasht);
	free(hash_by_revscode);
}


hash_table *create_hasht() {
	int i;
	hash_table *hasht = malloc(sizeof(hash_table));
	hasht -> entries = malloc(sizeof(hash_entry)*HASH_TABLE_SIZE);
	for (i = 0; i < HASH_TABLE_SIZE; i++) {
		hasht -> entries[i] = NULL;
	}
	return hasht;
}


/*
hash_entry *create_entry(reservation *reserv){
	hash_entry *entry = malloc(sizeof(hash_entry));
	entry->reserv->reservation_code = malloc(sizeof(char)*
	strlen(reserv->reservation_code));
	puts("C");
	entry->reserv = reserv;
	puts("D");
	entry->next = NULL;
	return entry;
}
*/


void hasht_insert(hash_table *hasht, reservation *reserv, int key_switch){
	unsigned int n_entry;
	hash_entry *entry;
	hash_entry *new_entry;
	if(key_switch)
		n_entry = hash_func(reserv->flight_id);
	else
		n_entry = hash_func(reserv->reservation_code);

	entry = hasht->entries[n_entry];
	new_entry = malloc(sizeof(hash_entry));
	new_entry->reserv = reserv;
	new_entry->next = NULL; 
	if (entry == NULL){
		hasht->entries[n_entry] = new_entry;
	}
	else {
		while(entry->next != NULL){
			entry = entry->next;
		}
		entry->next = new_entry;
	}

}


hash_entry *hasht_get(hash_table *hasht, char flight_id[FLIGHT_CODE_LEN]){
	unsigned int n_entry = hash_func(flight_id);
	hash_entry *entry = hasht->entries[n_entry];
	if(entry == NULL){
		return NULL;
	}
	while(entry != NULL){
		if(!strcmp(entry->reserv->flight_id, flight_id))
			return entry;
		entry = entry->next;
	}
	return NULL;
}


void del(int pos, hash_entry *entry){
	hash_entry *temp = entry;
	hash_entry *del;
	int i;
	if(pos==0){
		entry=entry->next;
		temp->next = NULL;
		free(temp);
	}
	else{
		for(i=0; i<pos-1; i++){
			temp = temp->next;
		}
		del = temp->next;
		temp->next=temp->next->next;
		del->next=NULL;
		free(del);
	}
}

/*
int get_total_p(hash_table *hasht, char flight_id[FLIGHT_CODE_LEN]){
	int n_passengers = 0;
	hash_entry *entry = hasht_get(hasht, flight_id);
	while(entry != NULL){
		n_passengers += entry->reserv->n_passengers;
		entry=entry->next;
	}
	return n_passengers;
}
*/

hash_entry *hash_resvcode_get(hash_table *hasht, char *reservation_code){
	unsigned int n_entry = hash_func(reservation_code);
	hash_entry *entry = hasht->entries[n_entry];
	if(entry == NULL){
		return NULL;
	}
	while(entry != NULL){
		if(!strcmp(entry->reserv->reservation_code, reservation_code)){
			return entry;
		}
		entry = entry->next;
	}
	return NULL;
}


/*
void del_revscode(hash_entry **h_entry, hash_table *hash_by_revscode, 
	char *resvcode){
	unsigned int n_entry = hash_func(resvcode);
	hash_entry *entry1 = hash_by_revscode->entries[n_entry];
	hash_entry *prev = malloc(sizeof(hash_entry));
	while(entry1 != NULL){
		if(!strcmp(entry1->reserv->reservation_code, 
			entry->reserv->reservation_code)){
			entry1=entry1->next;
			puts("A");
			prev->next=entry1;
			puts("B");
		}
		prev = entry1;
		entry1 = entry1->next;
	}
}
*/

/*
void del_revscode(hash_table *hash_by_revscode, char *resvcode){
	hash_entry *head = hash_resvcode_get(hash_by_revscode, resvcode);
	hash_entry *temp = head, *prev;
	while(temp != NULL && !strcmp(temp->reserv->reservation_code, resvcode)){
		head = temp->next;
		free(temp);
		temp = head;
	}
	while(temp != NULL){
		while(temp != NULL && strcmp(temp->reserv->reservation_code, resvcode)){
			prev = temp;
			temp = temp->next;
		}
		if (temp == NULL)
			return;
		prev->next = temp->next;
		free(temp);
		temp = prev->next;
	}
}
*/

/*
void del_fl_id_hasht(hash_table *hasht, char *flight_id, char *fl_id){
	int n_entry = hash_func(fl_id);
	hash_entry *entry = hasht->entries[n_entry];
	hash_entry *prev = entry;
	int index = 0;
	if(entry == NULL)
		return;
	while(entry != NULL){
		if(!strcmp(entry->reserv->reservation_code, reserv_code)){
			if(entry->next == NULL && index == 0)
				hasht->entries[n_entry] = NULL;
			if(entry->next != NULL && index == 0)
				hasht->entries[n_entry] = entry->next;
			if(entry->next == NULL && index != 0)
				prev->next = NULL;
			if(entry->next != NULL && index != 0)
				prev->next = entry->next;
			free(entry);
			return;
		}
		prev = entry;
		entry = prev->next;
	}
}
*/


/* How to free entry ?? */
void del_revscode_hasht(hash_table *hasht, char *flight_id, char *reserv_code){
	int n_entry = hash_func(flight_id);
	hash_entry *entry = hasht->entries[n_entry];
	hash_entry *prev = entry;
	int index = 0;
	if(entry == NULL)
		return;
	while(entry != NULL){
		if(!strcmp(entry->reserv->reservation_code, reserv_code)){
			if(entry->next == NULL && index == 0)
				hasht->entries[n_entry] = NULL;
			if(entry->next != NULL && index == 0)
				hasht->entries[n_entry] = entry->next;
			if(entry->next == NULL && index != 0)
				prev->next = NULL;
			if(entry->next != NULL && index != 0)
				prev->next = entry->next;
			index--;
			free(entry);
			return;
		}
		prev = entry;
		entry = prev->next;
		++index;
	}
}


void del_revscode(hash_table *hash_by_revscode, char *revscode,
	hash_table *hasht){
	int n_entry = hash_func(revscode);
	hash_entry *entry = hash_by_revscode->entries[n_entry]; 
	hash_entry *prev = entry;
	int index = 0;
	if(entry == NULL)
		return;
	if(hasht != NULL)
		del_revscode_hasht(hasht, entry->reserv->flight_id, revscode);
	while(entry != NULL){
		if(!strcmp(entry->reserv->reservation_code, revscode)){
			if(entry->next == NULL && index == 0){
				hash_by_revscode->entries[n_entry] = NULL;
			}
			if(entry->next != NULL && index == 0){
				hash_by_revscode->entries[n_entry] = entry->next;
			}
			if(entry->next == NULL && index != 0){
				prev->next = NULL;
			}
			if(entry->next != NULL && index != 0){
				prev->next = entry->next;
			}
			free(entry->reserv->reservation_code);
			free(entry->reserv);
			free(entry);
			return;
		}
		prev = entry;
		entry = prev->next;
		++index;
	}
}



void del_hasht_revscode(hash_table *hash_by_revscode, char *flight_id, 
	char *reserv_code){
	int n_entry = hash_func(reserv_code);
	hash_entry *entry = hash_by_revscode->entries[n_entry];
	hash_entry *prev = entry;
	int index = 0;
	if(entry == NULL)
		return;
	while(entry != NULL){
		if(!strcmp(entry->reserv->flight_id, flight_id)){
			if(entry->next == NULL && index == 0)
				hash_by_revscode->entries[n_entry] = NULL;
			if(entry->next != NULL && index == 0)
				hash_by_revscode->entries[n_entry] = entry->next;
			if(entry->next == NULL && index != 0)
				prev->next = NULL;
			if(entry->next != NULL && index != 0)
				prev->next = entry->next;
			/*
			free(entry->reserv->reservation_code);
			free(entry->reserv);
			*/
			free(entry);
			return;
			index--;
		}
		prev = entry;
		entry = prev->next;
		++index;
		/*
		if(!strcmp(prev->reserv->flight_id, flight_id)){
			if(prev!=NULL){
				free(prev->reserv->reservation_code);
				free(prev->reserv);
			}
		}
		*/
	}
}


void del_hasht(hash_table *hasht, char *flight_id, hash_table *hash_by_revscode){
	int n_entry = hash_func(flight_id);
	hash_entry *entry = hasht->entries[n_entry];
	hash_entry *prev = entry;
	int index = 0;
	if(entry == NULL)
		return;
	while(entry != NULL){
		del_hasht_revscode(hash_by_revscode, flight_id, entry->reserv->reservation_code);
		if(!strcmp(entry->reserv->flight_id, flight_id)){
			if(entry->next == NULL && index == 0)
				hasht->entries[n_entry] = NULL;
			if(entry->next != NULL && index == 0)
				hasht->entries[n_entry] = entry->next;
			if(entry->next == NULL && index != 0)
				hasht = NULL;
			if(entry->next != NULL && index != 0)
				prev->next = entry->next;
			index--;
		}
		prev = entry;
		entry = prev->next;
		++index;

	}
}


hash_entry* partition(hash_entry* head, hash_entry *end, hash_entry** new_head,
	hash_entry** new_end){
	hash_entry* temp;
	hash_entry* pivot = end;
	hash_entry *prev = NULL, *curr = head, *tail = pivot;

	while(curr != pivot){
		if(strcmp(curr->reserv->reservation_code, 
			pivot->reserv->reservation_code) < 0){
			if((*new_head) == NULL)
				(*new_head) = curr;
			prev = curr;
			curr = curr->next;
		}
		else {
			if(prev)
				prev->next = curr->next;
			temp = curr->next;
			curr->next = NULL;
			tail->next = curr;
			tail = curr;
			curr = temp;
		}
	}
	if((*new_head)==NULL)
		(*new_head) = pivot;
	(*new_end) = tail;
	return pivot;
}


hash_entry* get_end(hash_entry* curr){
	while(curr != NULL && curr->next != NULL)
		curr = curr->next;
	return curr;
}


hash_entry* quick_sort_aux(hash_entry* head, hash_entry *end){
	hash_entry *n_head = NULL, *n_end = NULL;
	hash_entry *pivot;
	hash_entry *temp;
	if(!head || head == end)
		return head;
	pivot = partition(head, end, &n_head, &n_end);

	if(n_head != pivot){
		temp = n_head;
		while(temp->next != pivot)
			temp = temp->next;
		temp->next = NULL;
		n_head = quick_sort_aux(n_head, temp);
		temp = get_end(n_head);
		temp->next = pivot;
	}
	pivot->next = quick_sort_aux(pivot->next, n_end);
	return n_head;
}


hash_entry *quick_sort_linked(hash_entry** head, hash_table *hasht, 
	char flight_id[FLIGHT_CODE_LEN]){
	int n_entry = hash_func(flight_id);
	(*head) = quick_sort_aux(*head, get_end(*head));
	hasht->entries[n_entry]=*head;

	return *head;
}

