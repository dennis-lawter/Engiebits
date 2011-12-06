/*
 * ----------------------------------------------------------------
 * Simple data structures provided by engine
 * ----------------------------------------------------------------
 */

#include "NGB/ngb.h"
#include "NGB/ngb_internal.h"

#include <string.h>

unsigned char _pearsonHashTab[256];

//Linked List
NGBLL* ngbLL_create(void) {
	NGBLL* list = malloc(sizeof(NGBLL));
	list->size = 0;
	list->head = NULL;
	list->tail = NULL;
	return list;
}
void ngbLL_insertFront(NGBLL* list, void* newContent) {
	NGBLL_NODE* newNode = malloc(sizeof(NGBLL_NODE));
	newNode->content = newContent;
	newNode->next = list->head;
	newNode->last = NULL;
	if (list->size > 0) {
		list->head->last = newNode;
		list->head = newNode;
	} else {
		list->head = newNode;
		list->tail = newNode;
	}
	list->size++;
}
void ngbLL_insertBack(NGBLL* list, void* newContent) {
	NGBLL_NODE* newNode = malloc(sizeof(NGBLL_NODE));
	newNode->content = newContent;
	newNode->next = NULL;
	newNode->last = list->tail;
	if (list->size > 0) {
		list->tail->next = newNode;
		list->tail = newNode;
	} else {
		list->head = newNode;
		list->tail = newNode;
	}
	list->size++;
}
void* ngbLL_removeFront(NGBLL* list) {
	if (list->size > 0) {
		NGBLL_NODE* resultNode;
		void* result;
		resultNode = list->head;
		result = resultNode->content;

		if (list->size > 1) {
			list->head = list->head->next;
			list->head->last = NULL;
		} else {
			list->head = NULL;
			list->tail = NULL;
		}
		list->size--;

		free(resultNode);
		return result;
	} else {
		return NULL;
	}
}
void* ngbLL_removeBack(NGBLL* list) {
	if (list->size > 0) {
		NGBLL_NODE* resultNode;
		void* result;
		resultNode = list->tail;
		result = resultNode->content;

		if (list->size > 1) {
			list->tail = resultNode->last;
			list->tail->next = NULL;
		} else {
			list->head = NULL;
			list->tail = NULL;
		}
		list->size--;

		free(resultNode);
		return result;
	} else {
		return NULL;
	}
}
void* ngbLL_peakFront(NGBLL* list) {
	return list->head->content;
}
void* ngbLL_peakBack(NGBLL* list) {
	return list->tail->content;
}
NGBLL_NODE* _ngbLL_find(NGBLL* list, NGBuint key) {
	NGBLL_NODE* findNode;
	int i;
	if (key > list->size / 2) {
		findNode = list->tail;
		for (i = list->size; i > key + 1; i--) {
			findNode = findNode->last;
		}
	} else {
		findNode = list->head;
		for (i = 0; i < key; i++) {
			findNode = findNode->next;
		}
	}
	return findNode;
}
void ngbLL_insertAt(NGBLL* list, NGBuint key, void* newContent) {
	if (key == 0) {
		ngbLL_insertFront(list, newContent);
	} else if (key == list->size) {
		ngbLL_insertBack(list, newContent);
	} else {
		NGBLL_NODE* newNode = malloc(sizeof(NGBLL_NODE));
		newNode->content = newContent;

		NGBLL_NODE* newLast = _ngbLL_find(list, key - 1);
		NGBLL_NODE* newNext = newLast->next;

		newNode->last = newLast;
		newNode->next = newNext;

		newLast->next = newNode;
		newNext->last = newNode;

		list->size++;
	}
}
void* ngbLL_removeAt(NGBLL* list, NGBuint key) {
	if (list->size < 1) {
		return NULL;
	}
	if (key == 0) {
		return ngbLL_removeFront(list);
	} else if (key == list->size - 1) {
		return ngbLL_removeBack(list);
	} else {
		NGBLL_NODE* findNode = _ngbLL_find(list, key);
		findNode->last->next = findNode->next;
		findNode->next->last = findNode->last;
		void* content = findNode->content;
		free(findNode);
		return content;
	}
}
void* ngbLL_peakAt(NGBLL* list, NGBuint key) {
	if (list->size < 1) {
		return NULL;
	} else if (key == 0) {
		return ngbLL_peakFront(list);
	} else if (key == list->size - 1) {
		return ngbLL_peakBack(list);
	}
	NGBLL_NODE* findNode = _ngbLL_find(list, key);
	return findNode->content;
}
void** ngbLL_toArray(NGBLL* list) {
	if (list->size > 0) {
		void** result = malloc(list->size * sizeof(void*));
		NGBLL_NODE* current = list->head;
		int i;
		for (i = 0; current != NULL; i++) {
			result[i] = current->content;
			current = current->next;
		}
		return result;
	} else {
		return NULL;
	}
}
void ngbLL_destroy(NGBLL* list) {
	while (list->size > 0) {
		free(ngbLL_removeFront(list));
	}
	free(list);
}

//Dynamic Array

NGBDA* ngbDA_create(void) {
	//TODO
	return NULL;
}
void ngbDA_set(NGBDA* dynarr, NGBuint key, void* value) {
	//TODO
}
void* ngbDA_get(NGBDA* dynarr, NGBuint key) {
	//TODO
	return NULL;
}
void ngbDA_destroy(NGBDA* dynarr) {
	//TODO
}

//Associative Array

NGBAA* ngbAA_create(void) {
	//TODO
	return NULL;
}
void ngbAA_set(NGBAA* assarr, char* key, void* value) {
	//TODO
}
void* ngbAA_get(NGBAA* assarr, char* key) {
	//TODO
	return NULL;
}
void ngbAA_destroy(NGBAA* assarr) {
	//TODO
}

//Hash Table
struct _NGBHT_NODE {
	void* content;
	char* key;
}typedef _NGBHT_NODE;

NGBHT* ngbHT_create(void) {
	NGBHT* table = malloc(sizeof(NGBHT));
	int i;
	for (i = 0; i < 256; i++) {
		table->buckets[i] = ngbLL_create();
	}
	return table;
}
void ngbHT_insert(NGBHT* table, char* key, void* data) {
	//TODO: Override old data if same key inserted
	unsigned char hash = ngbHT_hash(key);
	_NGBHT_NODE* newNode = malloc(sizeof(_NGBHT_NODE));
	newNode->key = key;
	newNode->content = data;
	ngbLL_insertFront(table->buckets[hash], newNode);
}
void* ngbHT_get(NGBHT* table, char* key) {
	unsigned char hash = ngbHT_hash(key);
	_NGBHT_NODE* node;
	int i;
	for (i = 0; i < table->buckets[hash]->size; i++) {
		node = ngbLL_removeFront(table->buckets[hash]);
		if (strcmp(node->key, key) == 0) {
			return node->content;
		}
	}
	return NULL;
}
void _ngbHT_hashInit(void) {
	int i, j;
	char temp;
	for (i = 0; i < 256; i++) {
		_pearsonHashTab[i] = (unsigned char) i;
	}
	for (i = 255; i > 0; i--) {
		j = rand() % i + 1;
		temp = _pearsonHashTab[i];
		_pearsonHashTab[i] = _pearsonHashTab[j];
		_pearsonHashTab[j] = temp;
	}
}
unsigned char ngbHT_hash(char* key) {
	unsigned char h = 0;
	int i;
	for (i = 0; key[i] != '\0'; i++) {
		h = _pearsonHashTab[h ^ key[i]];
	}
	return h;
}
void ngbHT_destroy(NGBHT* table) {
	int i;
	for (i = 0; i < 256; i++) {
		while (table->buckets[i]->size > 0) {
			_NGBHT_NODE* node = ngbLL_removeFront(table->buckets[i]);
			free(node->content);
			free(node->key);
			free(node);
		}
		ngbLL_destroy(table->buckets[i]);
	}
	free(table);
}
