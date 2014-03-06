/*
 * gengdic.c
 *
 * Gtk English dictionary for korean
 *
 * Author: Oh Junseon <hollywar@mail.holywar.net>
 * Date  : $Date: 2005/10/10 04:50:23 $ 
 *
 * Author: shyblue <shyblue@ucsoft.net>
 * Date  : 2005/09/27
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
#ifdef HAVE_CONFIG_H
#include    <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h> // for get file size //

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <glade/glade.h>
#include <glade/glade-xml.h>

#include <locale.h>

#include <zlib.h>  // gzip library //

#include "gengdic.h"
#include "eggtrayicon.h"

#ifdef _DATADIR_
#define DATADIR _DATADIR_
#endif

GladeXML        *gp_main_xml;
char            tray_icon_embedded =0 ;
char            is_tray=0;
GtkWidget *about_widget;

gboolean on_search_key_press_event( GtkWidget *widget, GdkEventKey *event, gpointer *user_data )
{
    GtkWidget *tv_result;

    if(!(event->state & (GDK_SHIFT_MASK | GDK_CONTROL_MASK)) && event->keyval == GDK_Return)
    {
        gchar              *key_word;


        key_word = g_strdup(gtk_entry_get_text(GTK_ENTRY(widget)));
        gtk_entry_set_text(GTK_ENTRY(widget),"");

        find_word(key_word);

        g_free(key_word);
        return TRUE;
    }

    return FALSE;
}

gboolean on_main_win_delete_event(GtkObject *object, gpointer user_data)
{
    if ( tray_icon_embedded == 1)
    {
        gtk_widget_hide(GTK_WIDGET(object));
        is_tray=1;
    }
    else
    {
        on_quit();
    }

    return TRUE;
}

void on_about_activate(GtkObject *object, gpointer user_data)
{
    if( !about_widget )
    	about_widget = glade_xml_get_widget(gp_main_xml,"about1");
    g_signal_connect( G_OBJECT( about_widget ), "close", G_CALLBACK( on_about_delete ), NULL );
    gtk_widget_show( about_widget );
}

gboolean on_about_delete(GtkWidget *widget,GdkEvent *event, gpointer user_data)
{
    gtk_widget_hide( widget );
    return TRUE;
}

void on_exit_activate(GtkObject *object, gpointer user_data)
{
    on_quit();
}

void on_quit()
{
    gtk_main_quit();
}

void on_main_win_activate_default(GtkObject *object, gpointer user_data)
{
    //gtk_widget_set_focus();
}

void find_word(char *key_word) 
{
    char buf[COLS_MAX] ;
    char *small_key_word ;
    char *tk  , *small_tk;
    char first_char ;
    char sep[]=":";
    int file_size=0 , my_value = 0 , sv=1 , check = 0;
    int cnt=0;

    struct stat sb ;

    // type of gzip file //
    register gzFile *gzfp ;

    small_key_word = lower_string(key_word) ;

    if (han_check(key_word)) {
        error_msg("이번 버전에서는 한글찾기는 지원하지 않습니다.");
        return ; 
    }
    // get first keyword character for reading file //
    first_char = tolower(key_word[0]) ;
    sprintf(buf,"%s/engdic/%c.dic.gz",DATADIR,first_char) ;

    // open gzip file //
    gzfp = gzopen(buf,"rb");
    if (stat(buf,&sb)==-1)
    {
        error_msg("사전 자료를 여는데 실패했습니다.\n 설치정보를 확인하세요.") ;
        return ;
    }

    file_size = (int) sb.st_size ;
    my_value = file_size / 2  ;

    // Binary search algorism //
    if (strlen(key_word) > 1 && key_word[1] > (int)'c')
        while(1) {
            gzseek(gzfp,my_value,SEEK_CUR) ;
            gzgets(gzfp,buf,COLS_MAX) ;
            gzgets(gzfp,buf,COLS_MAX) ;
            if (tolower(buf[sv]) > small_key_word[sv] -1) my_value = 0 - abs((my_value/2)) ;
            else if (tolower(buf[sv]) < small_key_word[sv] -1) my_value = abs((my_value/2)) ;
            else break ;	

            if (my_value == 0) break ;
        }
    if (strlen(key_word) > 1 && key_word[1] > (int)'c' && tolower(buf[sv]) > small_key_word[sv]-1)
        while(1) {
            gzseek(gzfp,-2000,SEEK_CUR) ;
            gzgets(gzfp,buf,COLS_MAX) ;
            gzgets(gzfp,buf,COLS_MAX) ;
            if (tolower(buf[sv]) > small_key_word[sv] -1) continue ;
            else break ;
        }

    // Matching keyword //
    check=0 ;
    while( ( gzgets(gzfp, buf, COLS_MAX) ) ) {
        tk = strtok( buf,sep ) ;
        small_tk = lower_string(tk) ;

        // Data have one space (remove one space) //
        small_tk[strlen(small_tk)-1] = '\0' ;

        if (!strcmp(small_tk,small_key_word)) 
        {
            check=1;
            cnt++;
            ucs_gtk_show_result(tk,strtok(NULL,"\0"),cnt) ;
        }
        if (check && small_tk[2] > small_key_word[2]) 
        {
            free(small_tk) ;
            break ;
        }
        free(small_tk) ;
    }

    free(small_key_word) ;
    gzclose(gzfp) ;
}

void view_adjust_scroll(GtkWidget *tv_result,GtkTextBuffer *buffer)
{
    GtkWidget *widget;
    GtkAdjustment *adj;

    widget = glade_xml_get_widget(gp_main_xml,"scrwin");

    adj = GTK_TEXT_VIEW(tv_result)->vadjustment;
    gtk_adjustment_set_value( adj, adj->upper - adj->page_size);

    adj = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(widget));
    adj->value = adj->upper;
    gtk_scrolled_window_set_vadjustment(GTK_SCROLLED_WINDOW( widget ), adj);
}

void ucs_gtk_show_message(gchar *msg,int cnt)
{
    GtkWidget *tv_result;
    GtkTextBuffer   *disp;
    GtkTextIter     siter,eiter;

    tv_result = glade_xml_get_widget(gp_main_xml, "tv_result");
    disp = gtk_text_view_get_buffer(GTK_TEXT_VIEW(tv_result));

    gtk_text_buffer_get_start_iter(disp, &siter);
    gtk_text_buffer_get_end_iter(disp, &eiter);
    if( cnt == 1)
    {
        gtk_text_buffer_delete(disp,&siter,&eiter);
    }
    gtk_text_buffer_insert(disp, &eiter, msg, -1);
    view_adjust_scroll(tv_result,disp);
}

int ucs_gtk_show_result(char *tk , char *tk2,int cnt) 
{
    gchar           *tk2_msg = NULL;

    if (tk == NULL || tk2 == NULL) return ;


    tk2_msg = g_strdup_printf("[ %s ] %s",tk,tk2);

    ucs_gtk_show_message(tk2_msg,cnt);

    if (tk2_msg)
    {
        g_free(tk2_msg);
    }
    return 0 ;
}


char *lower_string (char *data)
{
    char *i, *j ;
    char *code ;

    code = (char *)malloc ((strlen(data)+1) * sizeof (char) );

    for (j=code, i=data; *i!='\0'; i++, j++) {
        if (*i != ' ')
            *j = tolower(*i);
        else *j = ' ' ;
    }
    *j = '\0';

    return code ;
}

int han_check (char *data)
{
    int i=0 ;

    for (i=0 ; data[i] != '\0' ; i++) {
        if (data[i] & 0x80) return 1 ;
    }
    return 0 ;
}

void error_msg (char *message)
{
    gchar *msg;

    msg = g_strdup_printf("%s\n",message);
    ucs_gtk_show_message(msg,1) ;
}

void tray_icon_menu_position_func (GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer data)
{

    GdkWindow   *window;
    GdkScreen       *screen;
    gint            width, height, menu_width, menu_height;
    gint            screen_width, screen_height;

    window  = GDK_WINDOW (GTK_WIDGET (data)->window);
    screen  = gdk_drawable_get_screen (GDK_DRAWABLE (window));

    screen_width    = gdk_screen_get_width (screen);
    screen_height   = gdk_screen_get_height (screen);

    gdk_window_get_origin (window, x, y);

    gdk_drawable_get_size (window, &width, &height);

    if (!GTK_WIDGET_REALIZED(menu)) {
        gtk_widget_realize(GTK_WIDGET(menu));
    }

    gdk_drawable_get_size(GDK_WINDOW(GTK_WIDGET(menu)->window), &menu_width, &menu_height);

    if (*y + height < screen_height / 2) {
        *y += height;
    } else {
        *y -= menu_height;
    }

    if (*x + menu_width > screen_width) {
        *x = screen_width - menu_width;
    }

}

gboolean on_tray_icon_button_press( GtkWidget *widget, GdkEventButton *event, gpointer data)
{
    switch (event->button)
    {
        GtkWidget *window_main;
        GtkWidget *popup_menu;

        case 1:
            window_main = glade_xml_get_widget(gp_main_xml,"main_win");
            if (is_tray)
            {
                gtk_window_present( GTK_WINDOW( window_main ) );
                is_tray=0;
            }
            else
            {
                gtk_widget_hide(  window_main );
                is_tray=1;
            }
        break;
        case 3:
            popup_menu=glade_xml_get_widget(gp_main_xml,"tray_menu");
            gtk_menu_popup(GTK_MENU( popup_menu ), NULL, NULL, tray_icon_menu_position_func, widget, event->button, event->time);
        break;
    }

    return FALSE;
}

void on_tray_icon_embedded(GtkWidget *widget, gpointer data)
{
    tray_icon_embedded = 1;
}
void on_tray_icon_destroyed(GtkWidget *widget, gpointer date)
{
    tray_icon_embedded = 0;
}

gboolean tray_icon(void)
{
    EggTrayIcon *tray_icon;
    GtkWidget *image;
    GtkWidget *eventbox;
    GdkPixbuf *pixbuf = NULL;
    gchar     *tray_icon_path=NULL;

    tray_icon = egg_tray_icon_new("Gtk English Dictionanry");

    eventbox = gtk_event_box_new();
    gtk_widget_show(eventbox);
    gtk_container_add(GTK_CONTAINER(tray_icon), eventbox);
    g_signal_connect(G_OBJECT(eventbox), "button-press-event", G_CALLBACK(on_tray_icon_button_press), NULL);

    tray_icon_path = g_strdup_printf("%s/pixmaps/gedic/gedic_tray_icon.png",DATADIR);
    pixbuf = gdk_pixbuf_new_from_file( tray_icon_path, NULL);

    if( tray_icon_path )
    {
        g_free( tray_icon_path );
        tray_icon_path = NULL;
    }

    image = gtk_image_new_from_pixbuf( gdk_pixbuf_scale_simple( pixbuf, 20, 20, GDK_INTERP_BILINEAR ) );
    gtk_container_add( GTK_CONTAINER( eventbox ), image );

    g_signal_connect( G_OBJECT( tray_icon ), "embedded", G_CALLBACK( on_tray_icon_embedded ), NULL );
    g_signal_connect( G_OBJECT( tray_icon ), "destroy", G_CALLBACK( on_tray_icon_destroyed ), NULL );

    gtk_widget_show_all( GTK_WIDGET( tray_icon ) );

    if( pixbuf )
    {
        g_object_unref( pixbuf );
    }
}

int main( int     argc, char    *argv[])
{
    char buf[40];
    setlocale(LC_ALL,"");
    bindtextdomain("gedic","./locale");

    gtk_init(&argc, &argv);

    sprintf(buf,"%s/engdic/ui_main.xml",DATADIR) ;
    gp_main_xml = glade_xml_new( buf, NULL, "gedic" );

    glade_xml_signal_autoconnect(gp_main_xml);

    tray_icon();

    gtk_main();

    return ( 0 );
}
