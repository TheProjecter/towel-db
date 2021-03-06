/* towel-db - A human readable database system.
 * Copyright (C) 2008  Andrew <i80and@gmail.com>
 * 
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TOWELDB_H
#define TOWELDB_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/statvfs.h>

/* Version of the database implemented */
#define TOWELDB_DB_VERSION 0

/* Various utility macros */
#define TOWELDB_MAX_KEY_LEN 255
#define TOWELDB_META "_db"

/* FIXME: This has to be changed for non-POSIX systems */
#define TOWELDB_PATH_JOIN '/'

/* Errors */
typedef char toweldb_err;
enum _toweldb_err
{
	toweldb_err_noerror = 0,		/* Everything's just dandy */
	toweldb_err_record_exists,		/* The record already exists */
	toweldb_err_write_error,
	toweldb_err_delete_error,
	toweldb_err_key_too_long,
	toweldb_err_versionmismatch,	/* The database is of a different version */
};

/* Linked list of record fields */
typedef struct _toweldb_tuple
{
	off_t key_loc;
	off_t value_end;
	unsigned int key_len;
	unsigned long value_len;
	char* key;
	char* value;
	struct _toweldb_tuple* next;
} toweldb_field_node;

/* Database */
/*! An opaque data structure that defines a database handle. */
typedef struct _toweldb_db *toweldb_db;

/* Record */
/*! An opaque data structure that defines a database record. */
typedef struct _toweldb_rec *toweldb_rec;

/* Database stuff */
	/*! Open a database located at path.  If mode is 'c', create it if needed.*/
toweldb_db toweldb_open( const char* path, const char mode );
	/*! Get the path to the database root. */
char* toweldb_get_path( toweldb_db db );
	/*! Get the POSIX data handle on the database root directory.  This is a
	 * temperary hack and will be removed ASAP. */
DIR** toweldb_get_db_dir( toweldb_db db );
	/*! Get the maximum length of a filename for the filesystem that the
	 * database is on. */
char toweldb_get_max_key_len( toweldb_db db );
	/*! Delete the database.  WARNING: BROKEN */
toweldb_err toweldb_drop( toweldb_db db );
	/*! Close a database and free the memory storing it. */
void toweldb_close( toweldb_db db );
	
/* Record functions */
	/*! Get the number of records in the database. */
unsigned int toweldb_get_num_recs( toweldb_db db );
	/*! Get the modification time of a record. */
time_t toweldb_record_get_time( toweldb_rec rec );
	/*! Get the database that owns a record. */
toweldb_db* toweldb_get_record_parent( toweldb_rec rec );
	/*! Get the next key in the database.  This is a wrapper around the POSIX
	 * readdir that skips entries that the programmer doesn't need.  It will
	 * return NULL and rewind if it hits the last item in the directory. */
char* toweldb_get_next_key( toweldb_db db );
	/*! Create a new record within the database with the given key. */
toweldb_err toweldb_create_rec( toweldb_db db, const char* key );
	/*! Remove the record with name key from the database. */
toweldb_err toweldb_remove_rec( toweldb_db db, const char* key );

/* Record parsing functions */
/*! Read the record specified by key. */
toweldb_rec toweldb_read_rec( toweldb_db db, const char* key );
	/*! Free the record. */
void toweldb_free_rec( toweldb_rec rec );

#endif
