/*
 * ----------------------------------------------------------------
 * Game database
 * ----------------------------------------------------------------
 */

#include "NGB/ngb.h"
#include "NGB/ngb_internal.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//TODO: All this code is prototyping, most of it probably does not work yet.

NGBdatabase* ngbSQL_Init(void) {
	sqlite3* database;

	int err = sqlite3_open("data/db/sql.db", &database);
	if (err != SQLITE_OK) {
		return NULL;
	}

	sqlite3_close(database);
	return database;
}

NGBboolean ngbSQL_UpdateQuery(NGBdatabase* database, char* query) {
	sqlite3_stmt* statement;
	NGBboolean result;
	int err = sqlite3_prepare_v2(database, query, strlen(query) + 1, &statement,
			NULL);
	if (err != SQLITE_OK) {
		return NGB_FALSE;
	}

	result = sqlite3_step(statement) == SQLITE_DONE;

	return result;
}

NGBAA** ngbSQL_ResultQuery(NGBdatabase* database, char* query,
		int* sizeReturned) {
	NGBAA** results = NULL;
	sqlite3_stmt* statement;

	int err = sqlite3_prepare_v2(database, query, strlen(query) + 1, &statement,
			NULL);
	if (err != SQLITE_OK) {
		*sizeReturned = -1;
		return NULL;
	}

	while (1) {
		int s;
		int row = 0;
		s = sqlite3_step(statement);
		if (s == SQLITE_ROW) {
			const unsigned char * text;
			text = sqlite3_column_text(statement, 0);
			printf("%d: %s\n", row, text);
			row++;
		} else if (s == SQLITE_DONE) {
			break;
		} else {
			return NULL;
		}
	}

	*sizeReturned = -1;
	return results;
}

void ngbSQL_Destroy(NGBdatabase* database) {
	sqlite3_close(database);
	free(database);
}
