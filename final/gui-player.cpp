#include <gtk/gtk.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <netdb.h>
#include <algorithm>
#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;

GtkWidget *g_ip_address_box;
GtkWidget *g_port_box;
GtkWidget *g_connection_label;
GtkWidget *g_paper_button;
GtkWidget *g_scissors_button;
GtkWidget *g_rock_button;
GtkWidget *g_quit_button;
GtkWidget *g_status_label;
GtkWidget *g_p1_score_label;
GtkWidget *g_p2_score_label;

string ip;
string port;

int main(int argc, char *argv[])
{
    GtkBuilder      *builder; 
    GtkWidget       *window;
    
    // init window 
    gtk_init(&argc, &argv); 
    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "gui.glade", NULL); 
    window = GTK_WIDGET(gtk_builder_get_object(builder, "gui"));
    gtk_builder_connect_signals(builder, NULL);

    // labels and buttons
    g_connection_label = GTK_WIDGET(gtk_builder_get_object(builder, "connection_label"));
    g_paper_button = GTK_WIDGET(gtk_builder_get_object(builder, "paper_button"));
    g_scissors_button = GTK_WIDGET(gtk_builder_get_object(builder, "scissors_button"));
    g_rock_button = GTK_WIDGET(gtk_builder_get_object(builder, "rock_button"));
    g_quit_button = GTK_WIDGET(gtk_builder_get_object(builder, "quit_button"));
    g_status_label = GTK_WIDGET(gtk_builder_get_object(builder, "status_label"));
    g_p1_score_label = GTK_WIDGET(gtk_builder_get_object(builder, "p1_score_label"));
    g_p2_score_label = GTK_WIDGET(gtk_builder_get_object(builder, "p2_score_label"));

    // add text from boxes
    g_ip_address_box = GTK_ENTRY(gtk_builder_get_object(builder, "ip_address_box"));
    g_port_box = GTK_ENTRY(gtk_builder_get_object(builder, "port_box"));
 
    // standard build info 
    g_object_unref(builder); 
    gtk_widget_show(window);                
    gtk_main();
 
    return 0;
}

// receive ip address from box
static void ip_entered(){
  cout << ip << endl;
  ip = gtk_entry_get_text(g_ip_address_box);
  cout << ip << endl;
}

// receive port from box
static void port_entered(){
  cout << port << endl;
  port = gtk_entry_get_text(); 
  cout << port << endl;
}

// updated the not/connected status



// click rock button


// click scissors button


// click paper button


// update the status to "enter an answer or waiting"


// update player total points


// called when window is closed
void on_window_main_destroy()
{
    gtk_main_quit();
}
