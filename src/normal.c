/* vifm
 * Copyright (C) 2001 Ken Steen.
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */

#include <assert.h>
#include <string.h>

#include "background.h"
#include "bookmarks.h"
#include "cmdline.h"
#include "color_scheme.h"
#include "commands.h"
#include "config.h"
#include "filelist.h"
#include "fileops.h"
#include "keys.h"
#include "menus.h"
#include "modes.h"
#include "registers.h"
#include "search.h"
#include "status.h"
#include "ui.h"
#include "utils.h"
#include "visual.h"

#include "normal.h"

#define MAX(a,b) (((a) > (b)) ? (a) : (b))
#define MIN(a,b) (((a) < (b)) ? (a) : (b))

static int *mode;
static int last_search_backward;

static void init_extended_keys(void);
static void keys_ctrl_b(struct key_info, struct keys_info *);
static void keys_ctrl_c(struct key_info, struct keys_info *);
static void keys_ctrl_d(struct key_info, struct keys_info *);
static void keys_ctrl_e(struct key_info, struct keys_info *);
static void keys_ctrl_f(struct key_info, struct keys_info *);
static void keys_ctrl_g(struct key_info, struct keys_info *);
static void keys_tab(struct key_info, struct keys_info *);
static void keys_ctrl_l(struct key_info, struct keys_info *);
static void keys_return(struct key_info, struct keys_info *);
static void keys_ctrl_o(struct key_info, struct keys_info *);
static void keys_ctrl_u(struct key_info, struct keys_info *);
static void keys_ctrl_wl(struct key_info, struct keys_info *);
static void keys_ctrl_wh(struct key_info, struct keys_info *);
static void keys_ctrl_wo(struct key_info, struct keys_info *);
static void keys_ctrl_wv(struct key_info, struct keys_info *);
static void keys_ctrl_ww(struct key_info, struct keys_info *);
static void keys_ctrl_y(struct key_info, struct keys_info *);
static void keys_quote(struct key_info, struct keys_info *);
static void keys_percent(struct key_info, struct keys_info *);
static void keys_dot(struct key_info, struct keys_info *);
static void keys_colon(struct key_info, struct keys_info *);
static void keys_slash(struct key_info, struct keys_info *);
static void keys_question(struct key_info, struct keys_info *);
static void keys_G(struct key_info, struct keys_info *);
static void keys_H(struct key_info, struct keys_info *);
static void keys_L(struct key_info, struct keys_info *);
static void keys_M(struct key_info, struct keys_info *);
static void keys_N(struct key_info, struct keys_info *);
static void keys_V(struct key_info, struct keys_info *);
static void keys_ZQ(struct key_info, struct keys_info *);
static void keys_ZZ(struct key_info, struct keys_info *);
static void keys_cg(struct key_info, struct keys_info *);
static void keys_co(struct key_info, struct keys_info *);
static void keys_cw(struct key_info, struct keys_info *);
static void keys_dd(struct key_info, struct keys_info *);
static void keys_d_selector(struct key_info, struct keys_info *);
static void keys_gg(struct key_info, struct keys_info *);
static void keys_h(struct key_info, struct keys_info *);
static void keys_i(struct key_info, struct keys_info *);
static void keys_j(struct key_info, struct keys_info *);
static void keys_k(struct key_info, struct keys_info *);
static void keys_l(struct key_info, struct keys_info *);
static void keys_n(struct key_info, struct keys_info *);
static void keys_p(struct key_info, struct keys_info *);
static void keys_m(struct key_info, struct keys_info *);
static void keys_t(struct key_info, struct keys_info *);
static void keys_yy(struct key_info, struct keys_info *);
static void keys_y_selector(struct key_info, struct keys_info *);
static void yank_status_bar_msg(int count);
static void keys_zM(struct key_info, struct keys_info *);
static void keys_zO(struct key_info, struct keys_info *);
static void keys_zR(struct key_info, struct keys_info *);
static void keys_za(struct key_info, struct keys_info *);
static void keys_zb(struct key_info, struct keys_info *);
static void keys_zf(struct key_info, struct keys_info *);
static void keys_zm(struct key_info, struct keys_info *);
static void keys_zo(struct key_info, struct keys_info *);
static void keys_zt(struct key_info, struct keys_info *);
static void keys_zz(struct key_info, struct keys_info *);
static void pick_files(FileView *view, int end, struct keys_info *keys_info);

void
init_normal_mode(int *key_mode)
{
	struct key_t *curr;

	assert(key_mode != NULL);

	mode = key_mode;

	curr = add_keys(L"\x02", NORMAL_MODE);
	curr->data.handler = keys_ctrl_b;

	curr = add_keys(L"\x03", NORMAL_MODE);
	curr->data.handler = keys_ctrl_c;

	curr = add_keys(L"\x04", NORMAL_MODE);
	curr->data.handler = keys_ctrl_d;

	curr = add_keys(L"\x05", NORMAL_MODE);
	curr->data.handler = keys_ctrl_e;

	curr = add_keys(L"\x06", NORMAL_MODE);
	curr->data.handler = keys_ctrl_f;

	curr = add_keys(L"\x07", NORMAL_MODE);
	curr->data.handler = keys_ctrl_g;

	curr = add_keys(L"\x09", NORMAL_MODE);
	curr->data.handler = keys_tab;

	curr = add_keys(L"\x0c", NORMAL_MODE);
	curr->data.handler = keys_ctrl_l;

	curr = add_keys(L"\x0d", NORMAL_MODE);
	curr->data.handler = keys_return;

	curr = add_keys(L"\x0f", NORMAL_MODE);
	curr->data.handler = keys_ctrl_o;

	curr = add_keys(L"\x15", NORMAL_MODE);
	curr->data.handler = keys_ctrl_u;

	curr = add_keys(L"\x17\x17", NORMAL_MODE);
	curr->data.handler = keys_ctrl_ww;

	curr = add_keys(L"\x17\x08", NORMAL_MODE);
	curr->data.handler = keys_ctrl_wh;

	curr = add_keys(L"\x17h", NORMAL_MODE);
	curr->data.handler = keys_ctrl_wh;

	curr = add_keys(L"\x17\x0f", NORMAL_MODE);
	curr->data.handler = keys_ctrl_wo;

	curr = add_keys(L"\x17o", NORMAL_MODE);
	curr->data.handler = keys_ctrl_wo;

	curr = add_keys(L"\x17\x0c", NORMAL_MODE);
	curr->data.handler = keys_ctrl_wl;

	curr = add_keys(L"\x17l", NORMAL_MODE);
	curr->data.handler = keys_ctrl_wl;

	curr = add_keys(L"\x17\x16", NORMAL_MODE);
	curr->data.handler = keys_ctrl_wv;

	curr = add_keys(L"\x17v", NORMAL_MODE);
	curr->data.handler = keys_ctrl_wv;

	curr = add_keys(L"\x17w", NORMAL_MODE);
	curr->data.handler = keys_ctrl_ww;

	curr = add_keys(L"\x19", NORMAL_MODE);
	curr->data.handler = keys_ctrl_y;

	/* escape */
	curr = add_keys(L"\x1b", NORMAL_MODE);
	curr->data.handler = keys_ctrl_c;

	curr = add_keys(L"'", NORMAL_MODE);
	curr->type = BUILDIN_WAIT_POINT;
	curr->data.handler = keys_quote;
	curr->followed = FOLLOWED_BY_MULTIKEY;

	curr = add_keys(L" ", NORMAL_MODE);
	curr->data.handler = keys_tab;

	curr = add_keys(L"%", NORMAL_MODE);
	curr->data.handler = keys_percent;

	curr = add_keys(L".", NORMAL_MODE);
	curr->data.handler = keys_dot;

	curr = add_keys(L":", NORMAL_MODE);
	curr->data.handler = keys_colon;

	curr = add_keys(L"/", NORMAL_MODE);
	curr->data.handler = keys_slash;

	curr = add_keys(L"?", NORMAL_MODE);
	curr->data.handler = keys_question;

	curr = add_keys(L"G", NORMAL_MODE);
	curr->data.handler = keys_G;
	curr->selector = KS_SELECTOR_AND_CMD;

	curr = add_keys(L"H", NORMAL_MODE);
	curr->data.handler = keys_H;
	curr->selector = KS_SELECTOR_AND_CMD;

	curr = add_keys(L"L", NORMAL_MODE);
	curr->data.handler = keys_L;
	curr->selector = KS_SELECTOR_AND_CMD;

	curr = add_keys(L"M", NORMAL_MODE);
	curr->data.handler = keys_M;
	curr->selector = KS_SELECTOR_AND_CMD;

	curr = add_keys(L"N", NORMAL_MODE);
	curr->data.handler = keys_N;

	curr = add_keys(L"V", NORMAL_MODE);
	curr->data.handler = keys_V;

	curr = add_keys(L"Y", NORMAL_MODE);
	curr->data.handler = keys_yy;

	curr = add_keys(L"ZQ", NORMAL_MODE);
	curr->data.handler = keys_ZQ;

	curr = add_keys(L"ZZ", NORMAL_MODE);
	curr->data.handler = keys_ZZ;

	curr = add_keys(L"cg", NORMAL_MODE);
	curr->data.handler = keys_cg;

	curr = add_keys(L"co", NORMAL_MODE);
	curr->data.handler = keys_co;

	curr = add_keys(L"cw", NORMAL_MODE);
	curr->data.handler = keys_cw;

	curr = add_keys(L"dd", NORMAL_MODE);
	curr->data.handler = keys_dd;

	curr = add_keys(L"d", NORMAL_MODE);
	curr->data.handler = keys_d_selector;
	curr->type = BUILDIN_WAIT_POINT;
	curr->followed = FOLLOWED_BY_SELECTOR;

	curr = add_keys(L"gg", NORMAL_MODE);
	curr->data.handler = keys_gg;
	curr->selector = KS_SELECTOR_AND_CMD;

	curr = add_keys(L"h", NORMAL_MODE);
	curr->data.handler = keys_h;

	curr = add_keys(L"i", NORMAL_MODE);
	curr->data.handler = keys_i;

	curr = add_keys(L"j", NORMAL_MODE);
	curr->data.handler = keys_j;
	curr->selector = KS_SELECTOR_AND_CMD;

	curr = add_keys(L"k", NORMAL_MODE);
	curr->data.handler = keys_k;
	curr->selector = KS_SELECTOR_AND_CMD;

	curr = add_keys(L"l", NORMAL_MODE);
	curr->data.handler = keys_l;

	curr = add_keys(L"m", NORMAL_MODE);
	curr->type = BUILDIN_WAIT_POINT;
	curr->data.handler = keys_m;
	curr->followed = FOLLOWED_BY_MULTIKEY;

	curr = add_keys(L"n", NORMAL_MODE);
	curr->data.handler = keys_n;

	curr = add_keys(L"p", NORMAL_MODE);
	curr->data.handler = keys_p;

	curr = add_keys(L"t", NORMAL_MODE);
	curr->data.handler = keys_t;

	curr = add_keys(L"yy", NORMAL_MODE);
	curr->data.handler = keys_yy;

	curr = add_keys(L"y", NORMAL_MODE);
	curr->data.handler = keys_y_selector;
	curr->type = BUILDIN_WAIT_POINT;
	curr->followed = FOLLOWED_BY_SELECTOR;

	curr = add_keys(L"v", NORMAL_MODE);
	curr->data.handler = keys_V;

	curr = add_keys(L"zM", NORMAL_MODE);
	curr->data.handler = keys_zM;

	curr = add_keys(L"zO", NORMAL_MODE);
	curr->data.handler = keys_zO;

	curr = add_keys(L"zR", NORMAL_MODE);
	curr->data.handler = keys_zR;

	curr = add_keys(L"za", NORMAL_MODE);
	curr->data.handler = keys_za;

	curr = add_keys(L"zb", NORMAL_MODE);
	curr->data.handler = keys_zb;

	curr = add_keys(L"zf", NORMAL_MODE);
	curr->data.handler = keys_zf;

	curr = add_keys(L"zm", NORMAL_MODE);
	curr->data.handler = keys_zm;

	curr = add_keys(L"zo", NORMAL_MODE);
	curr->data.handler = keys_zo;

	curr = add_keys(L"zt", NORMAL_MODE);
	curr->data.handler = keys_zt;

	curr = add_keys(L"zz", NORMAL_MODE);
	curr->data.handler = keys_zz;

	init_extended_keys();
}

static void
init_extended_keys(void)
{
	struct key_t *curr;
	wchar_t buf[] = {L'\0', L'\0'};

	buf[0] = KEY_PPAGE;
	curr = add_keys(buf, NORMAL_MODE);
	curr->data.handler = keys_ctrl_b;

	buf[0] = KEY_NPAGE;
	curr = add_keys(buf, NORMAL_MODE);
	curr->data.handler = keys_ctrl_f;

	buf[0] = KEY_LEFT;
	curr = add_keys(buf, NORMAL_MODE);
	curr->data.handler = keys_h;

	buf[0] = KEY_DOWN;
	curr = add_keys(buf, NORMAL_MODE);
	curr->data.handler = keys_j;

	buf[0] = KEY_UP;
	curr = add_keys(buf, NORMAL_MODE);
	curr->data.handler = keys_k;

	buf[0] = KEY_RIGHT;
	curr = add_keys(buf, NORMAL_MODE);
	curr->data.handler = keys_l;
}

static void
keys_ctrl_b(struct key_info key_info, struct keys_info *keys_info)
{
	curr_view->list_pos -= curr_view->window_rows;
	moveto_list_pos(curr_view, curr_view->list_pos);
}

static void
keys_ctrl_c(struct key_info key_info, struct keys_info *keys_info)
{
	clean_selected_files(curr_view);
	redraw_window();
	curs_set(0);
}

static void
keys_ctrl_d(struct key_info key_info, struct keys_info *keys_info)
{
	curr_view->list_pos += curr_view->window_rows/2;
	moveto_list_pos(curr_view, curr_view->list_pos);
}

/* Scroll pane one line down. */
static void
keys_ctrl_e(struct key_info key_info, struct keys_info *keys_info)
{
	if(curr_view->list_rows <= curr_view->window_rows + 1)
		return;
	if(curr_view->top_line == curr_view->list_rows - curr_view->window_rows - 1)
		return;
	if(curr_view->list_pos == curr_view->top_line)
		curr_view->list_pos++;
	curr_view->top_line++;
	scroll_view(curr_view);
}

static void
keys_ctrl_f(struct key_info key_info, struct keys_info *keys_info)
{
	curr_view->list_pos += curr_view->window_rows;
	moveto_list_pos(curr_view, curr_view->list_pos);
}

static void
keys_ctrl_g(struct key_info key_info, struct keys_info *keys_info)
{
	if(!curr_stats.show_full)
		curr_stats.show_full = 1;
}

static void
keys_tab(struct key_info key_info, struct keys_info *keys_info)
{
	change_window();
}

/* Clear screen and redraw. */
static void
keys_ctrl_l(struct key_info key_info, struct keys_info *keys_info)
{
	redraw_window();
	curs_set(0);
}

static void
keys_return(struct key_info key_info, struct keys_info *keys_info)
{
	handle_file(curr_view, 0);
}

static void
keys_ctrl_o(struct key_info key_info, struct keys_info *keys_info)
{
	if(curr_view->history_pos == 0)
		return;

	goto_history_pos(curr_view, curr_view->history_pos - 1);
}

static void
keys_ctrl_u(struct key_info key_info, struct keys_info *keys_info)
{
	curr_view->list_pos -= curr_view->window_rows/2;
	moveto_list_pos(curr_view, curr_view->list_pos);
}

static void
keys_ctrl_wl(struct key_info key_info, struct keys_info *keys_info)
{
	if(curr_view->win == lwin.win)
		change_window();
}

static void
keys_ctrl_wh(struct key_info key_info, struct keys_info *keys_info)
{
	if(curr_view->win == rwin.win)
		change_window();
}

/* Leave only one pane. */
static void
keys_ctrl_wo(struct key_info key_info, struct keys_info *keys_info)
{
	comm_only();
}

/* To split pane. */
static void
keys_ctrl_wv(struct key_info key_info, struct keys_info *keys_info)
{
	comm_split();
}

static void
keys_ctrl_ww(struct key_info key_info, struct keys_info *keys_info)
{
	change_window();
}

static void
keys_ctrl_y(struct key_info key_info, struct keys_info *keys_info)
{
	if(curr_view->list_rows <= curr_view->window_rows + 1
			|| curr_view->top_line == 0)
		return;
	if(curr_view->list_pos == curr_view->top_line + curr_view->window_rows)
		curr_view->list_pos--;
	curr_view->top_line--;
	scroll_view(curr_view);
}

/* Jump to bottom of list or to specified line. */
static void
keys_G(struct key_info key_info, struct keys_info *keys_info)
{
	if(key_info.count == NO_COUNT_GIVEN)
		key_info.count = curr_view->list_rows;

	if(keys_info->selector)
		pick_files(curr_view, key_info.count - 1, keys_info);
	else
		moveto_list_pos(curr_view, key_info.count - 1);
}

/* Jump to top of the list or to specified line. */
static void
keys_gg(struct key_info key_info, struct keys_info *keys_info)
{
	if(key_info.count == NO_COUNT_GIVEN)
		key_info.count = 1;

	if(keys_info->selector)
		pick_files(curr_view, key_info.count - 1, keys_info);
	else
		moveto_list_pos(curr_view, key_info.count - 1);
}

/* Go to first file in window. */
static void
keys_H(struct key_info key_info, struct keys_info *keys_info)
{
	if(keys_info->selector)
	{
		pick_files(curr_view, curr_view->top_line, keys_info);
	}
	else
	{
		curr_view->list_pos = curr_view->top_line;
		moveto_list_pos(curr_view, curr_view->list_pos);
	}
}

/* Go to last file in window. */
static void
keys_L(struct key_info key_info, struct keys_info *keys_info)
{
	if(keys_info->selector)
	{
		pick_files(curr_view, curr_view->top_line + curr_view->window_rows,
				keys_info);
	}
	else
	{
		curr_view->list_pos = curr_view->top_line + curr_view->window_rows;
		moveto_list_pos(curr_view, curr_view->list_pos);
	}
}

/* Go to middle of window. */
static void
keys_M(struct key_info key_info, struct keys_info *keys_info)
{
	int new_pos;
	if(curr_view->list_rows < curr_view->window_rows)
	{
		new_pos = curr_view->list_rows/2;
	}
	else
	{
		new_pos = curr_view->top_line + (curr_view->window_rows/2);
	}
	if(keys_info->selector)
	{
		pick_files(curr_view, new_pos, keys_info);
	}
	else
	{
		curr_view->list_pos = new_pos;
		moveto_list_pos(curr_view, curr_view->list_pos);
	}
}

static void
keys_N(struct key_info key_info, struct keys_info *keys_info)
{
	if(cfg.search_history_num < 0)
		return;
	if(curr_view->selected_files == 0)
		find_pattern(curr_view, cfg.search_history[cfg.search_history_num],
				last_search_backward);

	if(last_search_backward)
		find_next_pattern(curr_view);
	else
		find_previous_pattern(curr_view);
}

/* Visual selection of files. */
static void
keys_V(struct key_info key_info, struct keys_info *keys_info)
{
	enter_visual_mode();
}

static void
keys_ZQ(struct key_info key_info, struct keys_info *keys_info)
{
	comm_quit();
}

static void
keys_ZZ(struct key_info key_info, struct keys_info *keys_info)
{
	comm_quit();
}

/* Mark. */
static void
keys_quote(struct key_info key_info, struct keys_info *keys_info)
{
	curr_stats.save_msg = get_bookmark(curr_view, key_info.multi);
}

/* Jump to percent of file. */
static void
keys_percent(struct key_info key_info, struct keys_info *keys_info)
{
	int line = (key_info.count * (curr_view->list_rows)/100);
	moveto_list_pos(curr_view, line - 1);
}

/* Repeat last change. */
static void
keys_dot(struct key_info key_info, struct keys_info *keys_info)
{
	if (0 > cfg.cmd_history_num)
		show_error_msg(" Command Error ", "Command history list is empty. ");
	else
		execute_command(curr_view, cfg.cmd_history[0]);
}

/* Command. */
static void
keys_colon(struct key_info key_info, struct keys_info *keys_info)
{
	enter_cmdline_mode(CMD_SUBMODE, L"", NULL);
}

/* Search forward. */
static void
keys_slash(struct key_info key_info, struct keys_info *keys_info)
{
	last_search_backward = 0;
	enter_cmdline_mode(SEARCH_FORWARD_SUBMODE, L"", NULL);
}

/* Search backward. */
static void
keys_question(struct key_info key_info, struct keys_info *keys_info)
{
	last_search_backward = 1;
	enter_cmdline_mode(SEARCH_BACKWARD_SUBMODE, L"", NULL);
}

/* Change group. */
static void
keys_cg(struct key_info key_info, struct keys_info *keys_info)
{
	change_group(curr_view);
}

/* Change owner. */
static void
keys_co(struct key_info key_info, struct keys_info *keys_info)
{
	change_owner(curr_view);
}

/* Change word (rename file). */
static void
keys_cw(struct key_info key_info, struct keys_info *keys_info)
{
	rename_file(curr_view);
}

/* Delete file. */
static void
keys_dd(struct key_info key_info, struct keys_info *keys_info)
{
	if(!curr_view->selected_files && key_info.count == NO_COUNT_GIVEN)
	{
		int x;
		int y = curr_view->list_pos;
		for(x = 0; x < key_info.count; x++)
		{
			curr_view->dir_entry[y].selected = 1;
			y++;
		}
	}
	if(key_info.reg == NO_REG_GIVEN)
		key_info.reg = DEFAULT_REG_NAME;
	delete_file(curr_view, key_info.reg, 0, NULL);
}

static void
keys_d_selector(struct key_info key_info, struct keys_info *keys_info)
{
	if(key_info.reg == NO_REG_GIVEN)
		key_info.reg = DEFAULT_REG_NAME;
	delete_file(curr_view, key_info.reg, keys_info->count, keys_info->indexes);

	free(keys_info->indexes);
	keys_info->indexes = NULL;
	keys_info->count = 0;
}

/* Updir. */
static void
keys_h(struct key_info key_info, struct keys_info *keys_info)
{
	change_directory(curr_view, "../");
	load_dir_list(curr_view, 0);
	moveto_list_pos(curr_view, curr_view->list_pos);
}

static void
keys_i(struct key_info key_info, struct keys_info *keys_info)
{
	handle_file(curr_view, 1);
}

/* Move down one line. */
static void
keys_j(struct key_info key_info, struct keys_info *keys_info)
{
	if(key_info.count == NO_COUNT_GIVEN)
		key_info.count = 1;
	if(keys_info->selector)
	{
		pick_files(curr_view, curr_view->list_pos + key_info.count, keys_info);
	}
	else
	{
		curr_view->list_pos += key_info.count;
		moveto_list_pos(curr_view, curr_view->list_pos);
	}
}

/* Move up one line. */
static void
keys_k(struct key_info key_info, struct keys_info *keys_info)
{
	if(key_info.count == NO_COUNT_GIVEN)
		key_info.count = 1;
	if(keys_info->selector)
	{
		pick_files(curr_view, curr_view->list_pos - key_info.count, keys_info);
	}
	else
	{
		curr_view->list_pos -= key_info.count;
		moveto_list_pos(curr_view, curr_view->list_pos);
	}
}

static void
keys_l(struct key_info key_info, struct keys_info *keys_info)
{
	handle_file(curr_view, 0);
}

/* Set mark. */
static void
keys_m(struct key_info key_info, struct keys_info *keys_info)
{
	add_bookmark(key_info.multi, curr_view->curr_dir,
			get_current_file_name(curr_view));
}

static void
keys_n(struct key_info key_info, struct keys_info *keys_info)
{
	if(cfg.search_history_num < 0)
		return;
	if(curr_view->selected_files == 0)
		find_pattern(curr_view, cfg.search_history[cfg.search_history_num],
				last_search_backward);

	if(last_search_backward)
		find_previous_pattern(curr_view);
	else
		find_next_pattern(curr_view);
}

/* Put files. */
static void
keys_p(struct key_info key_info, struct keys_info *keys_info)
{
	if(key_info.reg == NO_REG_GIVEN)
		key_info.reg = DEFAULT_REG_NAME;
	redraw_window();
	curr_stats.save_msg = put_files_from_register(curr_view, key_info.reg);
}

/* Tag file. */
static void
keys_t(struct key_info key_info, struct keys_info *keys_info)
{
	if(curr_view->dir_entry[curr_view->list_pos].selected == 0)
	{
		/* The ../ dir cannot be selected */
		if (!strcmp(curr_view->dir_entry[curr_view->list_pos].name, "../"))
				return;

		curr_view->dir_entry[curr_view->list_pos].selected = 1;
		curr_view->selected_files++;
	}
	else
	{
		curr_view->dir_entry[curr_view->list_pos].selected = 0;
		curr_view->selected_files--;
	}

	draw_dir_list(curr_view, curr_view->top_line, curr_view->list_pos);
	wattron(curr_view->win, COLOR_PAIR(CURR_LINE_COLOR) | A_BOLD);
	mvwaddstr(curr_view->win, curr_view->curr_line, 0, " ");
	wattroff(curr_view->win, COLOR_PAIR(CURR_LINE_COLOR));
	wmove(curr_view->win, curr_view->curr_line, 0);
}

/* Yank file. */
static void
keys_yy(struct key_info key_info, struct keys_info *keys_info)
{
	int count;

	if(key_info.count != NO_COUNT_GIVEN)
	{
		pick_files(curr_view, curr_view->list_pos + key_info.count - 1, keys_info);
	}
	if(key_info.reg == NO_REG_GIVEN)
		key_info.reg = DEFAULT_REG_NAME;

	count = yank_files(curr_view, key_info.reg, keys_info->count,
			keys_info->indexes);

	if(key_info.count != NO_COUNT_GIVEN)
		free(keys_info->indexes);

	yank_status_bar_msg(count);
}

static void
keys_y_selector(struct key_info key_info, struct keys_info *keys_info)
{
	int count;
	if(key_info.reg == NO_REG_GIVEN)
		key_info.reg = DEFAULT_REG_NAME;
	count = yank_files(curr_view, key_info.reg, keys_info->count,
			keys_info->indexes);

	free(keys_info->indexes);
	keys_info->indexes = NULL;
	keys_info->count = 0;

	yank_status_bar_msg(count);
}

static void
yank_status_bar_msg(int count)
{
	char buf[32];
	snprintf(buf, sizeof(buf), " %d %s yanked.", count,
			count == 1 ? "file" : "files");
	status_bar_message(buf);
	curr_stats.save_msg = 1;
}

static void
keys_zM(struct key_info key_info, struct keys_info *keys_info)
{
	restore_filename_filter(curr_view);
	hide_dot_files(curr_view);
}

/* Remove filename filter. */
static void
keys_zO(struct key_info key_info, struct keys_info *keys_info)
{
	remove_filename_filter(curr_view);
}

/* Show all hidden files. */
static void
keys_zR(struct key_info key_info, struct keys_info *keys_info)
{
	remove_filename_filter(curr_view);
	show_dot_files(curr_view);
}

static void
keys_za(struct key_info key_info, struct keys_info *keys_info)
{
	toggle_dot_files(curr_view);
}

/* Redraw with file in bottom of list. */
static void
keys_zb(struct key_info key_info, struct keys_info *keys_info)
{
	if(curr_view->list_rows <= curr_view->window_rows + 1)
		return;

	if(curr_view->list_pos < curr_view->window_rows)
		return;

	curr_view->top_line = curr_view->list_pos - curr_view->window_rows;
	scroll_view(curr_view);
}

/* Filter selected files. */
static void
keys_zf(struct key_info key_info, struct keys_info *keys_info)
{
	filter_selected_files(curr_view);
}

/* Hide dot files. */
static void
keys_zm(struct key_info key_info, struct keys_info *keys_info)
{
	hide_dot_files(curr_view);
}

static void
keys_zo(struct key_info key_info, struct keys_info *keys_info)
{
	show_dot_files(curr_view);
}

/* Redraw with file in top of list. */
static void
keys_zt(struct key_info key_info, struct keys_info *keys_info)
{
	if(curr_view->list_rows <= curr_view->window_rows + 1)
		return;

	if(curr_view->list_rows - curr_view->list_pos >= curr_view->window_rows)
		curr_view->top_line = curr_view->list_pos;
	else
		curr_view->top_line = curr_view->list_rows - curr_view->window_rows;
	scroll_view(curr_view);
}

/* Redraw with file in center of list. */
static void
keys_zz(struct key_info key_info, struct keys_info *keys_info)
{
	if(curr_view->list_rows <= curr_view->window_rows + 1)
		return;

	if(curr_view->list_pos < curr_view->window_rows/2)
		curr_view->top_line = 0;
	else if(curr_view->list_pos > curr_view->list_rows - curr_view->window_rows/2)
		curr_view->top_line = curr_view->list_rows - curr_view->window_rows;
	else
		curr_view->top_line = curr_view->list_pos - curr_view->window_rows/2;
	scroll_view(curr_view);
}

static void
pick_files(FileView *view, int end, struct keys_info *keys_info)
{
	int delta, i, x;

	end = MAX(0, end);
	end = MIN(view->list_rows - 1, end);

	keys_info->count = abs(view->list_pos - end) + 1;
	keys_info->indexes = calloc(keys_info->count, sizeof(int));
	if(keys_info->indexes == NULL)
	{
		show_error_msg(" Memory Error ", "Unable to allocate enough memory");
		return;
	}

	delta = (view->list_pos > end) ? -1 : +1;
	i = 0;
	x = view->list_pos - delta;
	do {
		x += delta;
		keys_info->indexes[i++] = x;
	} while(x != end);
}

/* vim: set tabstop=2 softtabstop=2 shiftwidth=2 noexpandtab : */
