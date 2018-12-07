#include <gtk/gtk.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

GtkEntry* g_ip_address_box;
GtkEntry* g_port_box;
GtkWidget* g_connect_button;
GtkWidget* g_paper_button;
GtkWidget* g_scissors_button;
GtkWidget* g_rock_button;
GtkWidget* g_quit_button;
GtkLabel* g_status_label;
GtkLabel* g_p1_score_label;
GtkLabel* g_p2_score_label;
GtkLabel* g_choice_label;

char ip[100];
char port[100];
int sockfd;
int player;

// handlers
void connect_clicked();
void rock_clicked();
void paper_clicked();
void scissors_clicked();
void quit_clicked();
void play_game();

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
    g_port_box = GTK_ENTRY(gtk_builder_get_object(builder, "port_box"));
    g_ip_address_box = GTK_ENTRY(gtk_builder_get_object(builder, "ip_address_box"));
    g_connect_button = GTK_WIDGET(gtk_builder_get_object(builder, "connect_button"));
    g_paper_button = GTK_WIDGET(gtk_builder_get_object(builder, "paper_button"));
    g_scissors_button = GTK_WIDGET(gtk_builder_get_object(builder, "scissors_button"));
    g_rock_button = GTK_WIDGET(gtk_builder_get_object(builder, "rock_button"));
    g_quit_button = GTK_WIDGET(gtk_builder_get_object(builder, "quit_button"));
    g_status_label = GTK_LABEL(gtk_builder_get_object(builder, "status_label"));
    g_p1_score_label = GTK_LABEL(gtk_builder_get_object(builder, "p1_score_label"));
    g_p2_score_label = GTK_LABEL(gtk_builder_get_object(builder, "p2_score_label"));
    g_choice_label = GTK_LABEL(gtk_builder_get_object(builder, "choice_label"));
    // add text from boxes
    g_ip_address_box = GTK_ENTRY(gtk_builder_get_object(builder, "ip_address_box"));
    g_port_box = GTK_ENTRY(gtk_builder_get_object(builder, "port_box"));
 
    // standard build info 
    g_object_unref(builder); 
    gtk_widget_show(window);                
    gtk_main();
 
    return 0;
}

// connected status
void connect_clicked(){
  printf("Connecting\n");
  const gchar* Gport = gtk_entry_get_text(g_port_box);
  sprintf(port, "%s", Gport);
  const gchar* Gip = gtk_entry_get_text(g_ip_address_box);
  sprintf(ip, "%s", Gip);
  printf("IP: %s\n", ip);
  printf("PORT: %s\n", port);

  // make a connection to that socket and update the global file socket variable
    int len;
    struct addrinfo *r;
    int nread, result;
    // use getaddrinfo for ease
    getaddrinfo(ip, port, NULL, &r);

    // use r datastructure to make socket
    sockfd = socket(r->ai_family, r->ai_socktype, r->ai_protocol);

    // make connection
    result = connect(sockfd, r->ai_addr, r->ai_addrlen);
    if(result == -1) {
        printf("the client failed to connect to the server\n");
        return;
    }

    // update the button to connected
    gtk_widget_set_name(g_connect_button, "Connected");
    
    // send them we want to play
    char* userInput = "READY";
    write(sockfd, userInput, strlen(userInput)+1);
    char buffer[256];
    int buflen = sizeof(buffer);
    // listen for GO
    nread = read(sockfd, buffer, buflen);
    // clt+d was pressed end the program
    if (strlen(buffer) == 0){
      gtk_label_set_text(g_choice_label, "The other player has exited!"); 
    }
    if(strcmp(buffer,"STOP")==0){
      gtk_label_set_text(g_choice_label, "The other player has exited!");
    }

    // tell the user which player they are
    nread = read(sockfd, buffer, buflen);
    if(strcmp(buffer,"1") == 0){
      gtk_label_set_text(g_choice_label, "Player 1: Make your move!"); 
      player = 1;
    }
    else{
      gtk_label_set_text(g_choice_label, "Player 2: Make your move!");
      player = 2;
    }
}

// rock
void rock_clicked(){
  char* userInput = "1";
  write(sockfd, userInput, strlen(userInput)+1);
  gtk_label_set_text(g_status_label, "Waiting on other player...");
  play_game();
}

// quit clicked
void quit_clicked(){
  char* userInput = "0";
  write(sockfd, userInput, strlen(userInput)+1);
  gtk_label_set_text(g_status_label, "Waiting on other player...");
  play_game();
}

// click scissors button
void scissors_clicked(){
  char* userInput = "3";
  write(sockfd, userInput, strlen(userInput)+1);
  gtk_label_set_text(g_status_label, "Waiting on other player...");
  play_game();
}


// click paper button
void paper_clicked(){
  char* userInput = "2";
  write(sockfd, userInput, strlen(userInput)+1);
  gtk_label_set_text(g_status_label, "Waiting on other player...");
  play_game();
}

// waits for user input
void play_game(){
  int nread;
  char buffer[256];
  int buflen = sizeof(buffer);
  // the result of the last game
  nread = read(sockfd, buffer, buflen);

  // see if they want to stop
  if (strcmp(buffer,"STOP") == 0){
    gtk_label_set_text(g_status_label, "GAME COMPLETE!  VIEW THE TOTAL SCORES BELOW");
  }

  // otherwise display the results
  else{
    if(strcmp(buffer,"1") == 0){
      gtk_label_set_text(g_status_label, "PLAYER 1 IS VICTORIOUS!");
      gtk_label_set_text(g_p1_score_label, "nice");
    }
    else if(strcmp(buffer,"2") == 0){
      gtk_label_set_text(g_status_label, "PLAYER 2 IS VICTORIOUS!");
      gtk_label_set_text(g_p2_score_label, "bad");
    }
    else
     gtk_label_set_text(g_status_label, "THERE WAS A TIE");
  } 
  
}


// called when window is closed
void on_window_main_destroy()
{
    close(sockfd);
    gtk_main_quit();
}
