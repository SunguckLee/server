/*****************************************************************************

Copyright (C) 2013, 2014 Facebook, Inc. All Rights Reserved.

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; version 2 of the License.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Suite 500, Boston, MA 02110-1335 USA

*****************************************************************************/

#ifndef btr0defragment_h
#define btr0defragment_h

#include "univ.i"

#ifndef UNIV_HOTBACKUP

#include "btr0pcur.h"

/* Max number of pages to consider at once during defragmentation. */
#define BTR_DEFRAGMENT_MAX_N_PAGES	32

/** stats in btr_defragment */
extern ulint btr_defragment_compression_failures;
extern ulint btr_defragment_failures;
extern ulint btr_defragment_count;

extern ulint btr_ttl_expire_scan_rows;
extern ulint btr_ttl_expired_rows;
extern ulint btr_skip_ttl_expire_pages;
extern ulint btr_skip_ttl_expire_rows;

/** Item in the work queue for btr_degrament_thread. */
struct btr_defragment_item_t
{
	btr_pcur_t*	pcur;		/* persistent cursor where
					btr_defragment_n_pages should start */
	os_event_t	event;		/* if not null, signal after work
					is done */
	bool		removed;	/* Mark an item as removed */
	ulonglong	last_processed;	/* timestamp of last time this index
					is processed by defragment thread */

	btr_defragment_item_t(btr_pcur_t* pcur, os_event_t event);
	~btr_defragment_item_t();
};

/******************************************************************//**
Initialize defragmentation. */
void
btr_defragment_init(void);
/******************************************************************//**
Shutdown defragmentation. */
void
btr_defragment_shutdown();
/******************************************************************//**
Check whether the given index is in btr_defragment_wq. */
bool
btr_defragment_find_index(
	dict_index_t*	index);	/*!< Index to find. */
/******************************************************************//**
Add an index to btr_defragment_wq. Return a pointer to os_event if this
is a synchronized defragmentation. */
os_event_t
btr_defragment_add_index(
	dict_index_t*	index,	/*!< index to be added  */
	bool		async);	/*!< whether this is an async defragmentation */
/******************************************************************//**
When table is dropped, this function is called to mark a table as removed in
btr_efragment_wq. The difference between this function and the remove_index
function is this will not NULL the event. */
void
btr_defragment_remove_table(
	dict_table_t*	table);	/*!< Index to be removed. */
/******************************************************************//**
Mark an index as removed from btr_defragment_wq. */
void
btr_defragment_remove_index(
	dict_index_t*	index);	/*!< Index to be removed. */
/*********************************************************************//**
Check whether we should save defragmentation statistics to persistent storage.*/
UNIV_INTERN
void
btr_defragment_save_defrag_stats_if_needed(
	dict_index_t*	index);	/*!< in: index */
/******************************************************************//**
Thread that merges consecutive b-tree pages into fewer pages to defragment
the index. */
extern "C" UNIV_INTERN
os_thread_ret_t
DECLARE_THREAD(btr_defragment_thread)(
/*==========================================*/
	void*	arg);		/*!< in: a dummy parameter required by
				os_thread_create */

#endif /* !UNIV_HOTBACKUP */
#endif
