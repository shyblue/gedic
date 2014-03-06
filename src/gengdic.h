/*
 * engdic.h
 *
 * English dictionary for korean
 *
 * Author: Oh Junseon <hollywar@mail.holywar.net>
 * Date  : $Date: 2005/10/05 07:49:54 $ 
 *
 * $Revision: 1.1.1.1 $
 *
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU General Public License for more details.
*/

// Global value

#ifndef INSTALL_DIC_DATA_DIR
#define INSTALL_DIC_DATA_DIR "/usr/share/engdic"
#endif
#ifndef INSTALL_PNG_DATA_DIR
#define INSTALL_PNG_DATA_DIR "/usr/share/pixmaps"
#endif
#define COLS_MAX 1024
gboolean on_search_key_press_event( GtkWidget *widget, GdkEventKey *event, gpointer *user_data );
gboolean on_main_win_delete_event(GtkObject *object, gpointer user_data);
void on_about_activate(GtkObject *object, gpointer user_data);
gboolean on_about_delete(GtkWidget *widget,GdkEvent *event, gpointer user_data);
void on_exit_activate(GtkObject *object, gpointer user_data);
void on_quit();
void on_main_win_activate_default(GtkObject *object, gpointer user_data);
void find_word(char *key_word) ;
void view_adjust_scroll(GtkWidget *tv_result,GtkTextBuffer *buffer);
void ucs_gtk_show_message(gchar *msg,int cnt);
int ucs_gtk_show_result(char *tk , char *tk2,int cnt) ;
char *lower_string (char *data);
int han_check (char *data);
void error_msg (char *message);
void tray_icon_menu_position_func (GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer data);
gboolean on_tray_icon_button_press( GtkWidget *widget, GdkEventButton *event, gpointer data);
void on_tray_icon_embedded(GtkWidget *widget, gpointer data);
void on_tray_icon_destroyed(GtkWidget *widget, gpointer date);
gboolean tray_icon(void);
