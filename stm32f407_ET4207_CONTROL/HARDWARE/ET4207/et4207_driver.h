/*
 * Copyright (C) 2012 Samsung Electronics
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */

#ifndef _ET4207_H_
#define _ET4207_H_

#define MAX_SEND_DATA 448

struct ir_remocon_data {
	struct mutex			mutex;

	uint8_t send_flag;
};

struct remote_desc {
	int gpio;
	char *name;	
	struct timer_list timer;
};


#endif /* _IR_REMOTE_ET4007_H_ */
