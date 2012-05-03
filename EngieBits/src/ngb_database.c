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
#include <sqlite3.h>

NGBlog* NGBlog_init(char* fileName, char* logStamp, NGBboolean timeStamp,
		NGBboolean openHeader) {
	NGBlog* log = (NGBlog*) malloc(sizeof(NGBlog));
	log->fileName = fileName;
	log->logStamp = logStamp;
	log->timeStamp = timeStamp;
	log->openHeader = openHeader;
	log->file = fopen(fileName, "a+");
	fseek(log->file, 0, SEEK_END);
	if (openHeader) {
		if (ftell(log->file) != 0) {
			fprintf(log->file, "\n");
		}
	}
}
void NGBlog_write(NGBlog* log, char* message) {
	time_t currentTime;
	struct tm* timeInfo;
	char* timestamp;

	time(&currentTime);
	timeInfo = localtime(&currentTime);
	timestamp = asctime(timeInfo);

	timestamp[strlen(timestamp)] = '\0';

	fprintf(log->file, "[%s]: %s\n", timestamp, message);
}
void NGBlog_newLine(NGBlog* log) {
	fprintf(log->file, "\n");
}
void NGBlog_lineBreak(NGBlog* log) {
	fprintf(
			log->file,
			"--------------------------------------------------------------------------------\n");
}
void NGBlog_destroy(NGBlog* log) {
	free(log->file);
	free(log->fileName);
	free(log->logStamp);
	free(log);
}

//TODO: All code below is prototyping, most of it probably does not work yet.

NGBSQL* NGBSQL_init(void) {
	sqlite3* database;

	int err = sqlite3_open("data/db/sql.db", &database);
	if (err != SQLITE_OK) {
		return NULL;
	}

	sqlite3_close(database);
	return database;
}

NGBboolean NGBSQL_updateQuery(NGBSQL* database, char* query) {
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

NGBAA** NGBSQL_resultQuery(NGBSQL* database, char* query, int* sizeReturned) {
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

void NGBSQL_destroy(NGBSQL* database) {
	sqlite3_close(database);
	free(database);
}
