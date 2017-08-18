#include <gtk/gtk.h> 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>

#ifndef DATA_PATH
#define DATA_PATH "./"
#endif

//These should work without root
static void shutdown(){
  system("systemctl poweroff");
}

static void suspend(){
  system("systemctl suspend");
}

static void reboot(){
  system("systemctl reboot");
}

static void activate (GtkApplication *app, gpointer user_data)
{
  //Screen to get monitor dimensions
  GdkScreen *screen = gdk_screen_get_default();

  //Widget for the main window
  GtkWidget *window;
  //Width for the grid view on the window
  GtkWidget *grid = gtk_grid_new();

  //Widgets for each button
  GtkWidget *powerOffButton;
  GtkWidget *restartButton;
  GtkWidget *suspendButton;
  GtkWidget *cancelButton;

  //Widgets for the button boxs containing the buttons
  GtkWidget *pob_box; 
  GtkWidget *rb_box;
  GtkWidget *sb_box;
  GtkWidget *cb_box;

  //Figure out how big a 'unit' should be on client monitor
  gint monitorWidth = gdk_screen_get_width(screen);
  gint monitorHeight = gdk_screen_get_height(screen);
  gint unit = monitorHeight/5;

  //Setup the main window
  window = gtk_application_window_new (app);
  gtk_window_set_title        (GTK_WINDOW (window), "Power");
  gtk_window_set_default_size (GTK_WINDOW (window), unit*4, unit);
  gtk_window_set_position     (GTK_WINDOW(window), GTK_WIN_POS_CENTER_ALWAYS); //Centered on screen
  gtk_window_set_decorated    (GTK_WINDOW(window), FALSE); //No title bar
  gtk_window_set_resizable    (GTK_WINDOW(window), FALSE); //Not resizable


  //Get images at correct size
  GtkWidget *power_buf    = gdk_pixbuf_new_from_file_at_size( DATA_PATH"images/pwr.png",
      unit/2, unit/2, NULL); 
  GtkWidget *restart_buf  = gdk_pixbuf_new_from_file_at_size( DATA_PATH"images/restart.png", 
      unit/2, unit/2, NULL);
  GtkWidget *suspend_buf  = gdk_pixbuf_new_from_file_at_size( DATA_PATH"images/suspend.png",  
      unit/2, unit/2, NULL);
  GtkWidget *cancel_buf   = gdk_pixbuf_new_from_file_at_size( DATA_PATH"images/cancel.png",   
      unit/2, unit/2, NULL);

  //Convert the images to gtk_image format to use on buttons
  GtkWidget *powerImg   = gtk_image_new_from_pixbuf(power_buf);
  GtkWidget *restartImg = gtk_image_new_from_pixbuf(restart_buf);
  GtkWidget *suspendImg = gtk_image_new_from_pixbuf(suspend_buf); 
  GtkWidget *cancelImg  = gtk_image_new_from_pixbuf(cancel_buf);

  //GTK Buttons must always show images
  GtkSettings *settings = gtk_settings_get_default();
  g_object_set(settings, "gtk-button-images", TRUE, NULL); 

  //Each grid box must be equal dimensions
  gtk_grid_set_row_homogeneous ( grid, TRUE );
  gtk_grid_set_column_homogeneous ( grid, TRUE );

  //Add grid to the main window
  gtk_container_add (GTK_CONTAINER (window), grid);

  //Button box orientation
  pob_box = gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL);
  rb_box  = gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL);
  sb_box  = gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL);
  cb_box  = gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL);

  //Attaching the button boxes to the grid
  gtk_grid_attach ( grid, pob_box, 0, 0, 1, 1);
  gtk_grid_attach ( grid, rb_box,  1, 0, 1, 1);
  gtk_grid_attach ( grid, sb_box,  2, 0, 1, 1);
  gtk_grid_attach ( grid, cb_box,  3, 0, 1, 1);

  //Create the buttons and give them each a label
  powerOffButton  = gtk_button_new_with_label ("Shut Down");
  restartButton   = gtk_button_new_with_label ("Restart");
  suspendButton   = gtk_button_new_with_label ("Suspend");
  cancelButton    = gtk_button_new_with_label ("Cancel");

  //Set the icon for each button
  gtk_button_set_image( powerOffButton, powerImg    );
  gtk_button_set_image( restartButton,  restartImg  );
  gtk_button_set_image( suspendButton,  suspendImg  );
  gtk_button_set_image( cancelButton,   cancelImg   );

  //Make the labels display beneath the icons
  gtk_button_set_image_position( powerOffButton,  GTK_POS_TOP );
  gtk_button_set_image_position( restartButton,   GTK_POS_TOP );
  gtk_button_set_image_position( suspendButton,   GTK_POS_TOP );
  gtk_button_set_image_position( cancelButton,    GTK_POS_TOP );

  //Set the size of the buttons
  gtk_widget_set_size_request( powerOffButton, (gint)unit/1.5, (gint)unit/1.5 );
  gtk_widget_set_size_request( restartButton, (gint)unit/1.5, (gint)unit/1.5 );
  gtk_widget_set_size_request( suspendButton, (gint)unit/1.5, (gint)unit/1.5 );
  gtk_widget_set_size_request( cancelButton, (gint)unit/1.5, (gint)unit/1.5 );

  //Set the action to be performed on button click
  g_signal_connect (powerOffButton, "clicked", G_CALLBACK (shutdown), NULL);
  g_signal_connect (restartButton,  "clicked",   G_CALLBACK (reboot), NULL);
  g_signal_connect (suspendButton,  "clicked",  G_CALLBACK (suspend), NULL);

  //Exit the program after clicking any button
  g_signal_connect_swapped (powerOffButton, "clicked", G_CALLBACK (gtk_widget_destroy), window);
  g_signal_connect_swapped (restartButton, "clicked", G_CALLBACK (gtk_widget_destroy), window);
  g_signal_connect_swapped (suspendButton, "clicked", G_CALLBACK (gtk_widget_destroy), window);
  g_signal_connect_swapped (cancelButton, "clicked", G_CALLBACK (gtk_widget_destroy), window);

  //Add the buttons to the button boxes
  gtk_container_add (GTK_CONTAINER (pob_box), powerOffButton );
  gtk_container_add (GTK_CONTAINER (rb_box),  restartButton );
  gtk_container_add (GTK_CONTAINER (sb_box),  suspendButton );
  gtk_container_add (GTK_CONTAINER (cb_box),  cancelButton );

  //Display everything
  gtk_widget_show_all (window);
}

int main (int argc,char **argv)
{
  //Crude way of consuming options - this shouldbe fixed later
  int opt_count = 0;

  //Handle cli flags
  if(argc > 1){

    //Template at https://www.gnu.org/software/libc/manual/html_node/Getopt-Long-Option-Example.html
    int get_opt_result;
    static int autosetup_flag = 0;

    while (1)
    {
      static struct option long_options[] =
      {
        /* These options set a flag. */
        {"auto-setup-power-button", no_argument, &autosetup_flag, 1},
        {0, 0, 0, 0}
      };
      /* getopt_long stores the option index here. */
      int option_index = 0;

      get_opt_result = getopt_long (argc, argv, "a",
          long_options, &option_index);

      /* Detect the end of the options. */
      if (get_opt_result == -1)
        break;

      switch (get_opt_result)
      {
        case 0:
          /* If this option set a flag, do nothing else now. */
          if (long_options[option_index].flag != 0)
            break;
          printf ("option %s", long_options[option_index].name);
          if (optarg)
            printf (" with arg %s", optarg);
          printf ("\n");
          break;
        case 'a':
          autosetup_flag = 0;
        case '?':
          /* getopt_long already printed an error message. */
          break;

        default:
          abort ();
      }
      opt_count++;
    }

    //Attempts to set the power button to open powermenu by default instead of whatever it currently does
    if(autosetup_flag == 1){
      FILE *f = fopen("/etc/systemd/logind.conf","r");
      if(f==NULL){
        printf("%s\n","Failed to open /etc/systemd/logind.conf. Do you have permission to read this file?");
        return 0;
      }
      char c = 255;
      static int pos = 0;
      static int MAX_FILE_LEN=20000;
      //Faster than malloc+memset
      char* tmp_file_buf = (char*)calloc(1,sizeof(char)*MAX_FILE_LEN+1);
      //Read file into local buffer
      do{
        c=fgetc(f);
        tmp_file_buf[pos] = c;
        pos++;
      }while(c != EOF && pos < MAX_FILE_LEN);
      tmp_file_buf[pos] = 0;
      fflush(f);
      fclose(f);

      //Find any instance of our keyword in the file
      //Try to make sure we find an uncommented version before a commented one
      char* str_file_ptr = strstr(tmp_file_buf, "\nHandlePowerKey=ignore");
      if(str_file_ptr == NULL)
        str_file_ptr = strstr(tmp_file_buf, "\nHandlePowerKey=");
      if(str_file_ptr == NULL)
        strstr(tmp_file_buf, "HandlePowerKey=");
      if(str_file_ptr != NULL && str_file_ptr[0] == '\n')
        str_file_ptr++;

      //Handle adding in our option string
      if(pos+strlen("HandlePowerKey=ignore\n") < MAX_FILE_LEN){
        //Append to the end
        if(str_file_ptr == NULL || str_file_ptr[-1] == '#'){
          //Don't add and extra '\n' if there is one already
          if(tmp_file_buf[pos-2] == '\n') pos--;
          strcpy((char*)((unsigned long)tmp_file_buf+pos-1), "\nHandlePowerKey=ignore\n");
          f = fopen("/etc/systemd/logind.conf","w");
          if(f==NULL){
            printf("%s\n","Failed to open /etc/systemd/logind.conf. Do you have permission to write to this file?");
            return 0;
          }
          fwrite(tmp_file_buf, sizeof(char), strlen(tmp_file_buf), f);
          fflush(f);
          fclose(f);
        }
        //Insert our string in the middle of the file to replace previous entry
        else{
          char* new_file = (char*)calloc(1,sizeof(char)*MAX_FILE_LEN+1);
          char* position = new_file;
          char* insert_str = "HandlePowerKey=ignore\n";
          strncpy(new_file, tmp_file_buf, ((unsigned long)str_file_ptr - (unsigned long)tmp_file_buf));
          position = (char*)((unsigned long)position+((unsigned long)str_file_ptr - (unsigned long)tmp_file_buf));
          strcpy(position, insert_str);
          position = position+strlen("HandlePowerKey=ignore\n");
          str_file_ptr = strchr(str_file_ptr,'\n');
          if(str_file_ptr != NULL){
            str_file_ptr++;
            strcpy(position, str_file_ptr);
            position[strlen(str_file_ptr)] = 0x0;
          }else{
            position[0] = 0x0;
          }
          f = fopen("/etc/systemd/logind.conf","w");
          if(f==NULL){
            printf("%s\n","Failed to open /etc/systemd/logind.conf. Do you have permission to write to this file?");
            return 0;
          } 
          fwrite(new_file, sizeof(char), strlen(new_file), f);
          fflush(f);
          fclose(f);
          free(new_file);
        } 
      }else{
        printf("Failed to modify /etc/systemd/logind.conf. Is your config file really long?\n");
        return 0;
      }
      free(tmp_file_buf);

      char* path = calloc(1,sizeof(char)*1000);
      char* runas = calloc(1, sizeof(char)*1000);	

      //Setup key binding 
      if(strcmp(getenv("USER"),"root") == 0){
        snprintf(path,1000,"printf \'#Power button binding\\n\"powermenu\"\\n\\tm:0x0 + c:124\\n\\tXF86PowerOff\\n\' >> /home/%s/.xbindkeysrc",getenv("SUDO_USER"));
        snprintf(runas,1000,"sudo -H -u %s bash -c 'xbindkeys'",getenv("SUDO_USER"));
      }else{
        snprintf(path,1000,"printf \'#Power button binding\\n\"powermenu\"\\n\\tm:0x0 + c:124\\n\\tXF86PowerOff\\n\' >> /home/%s/.xbindkeysrc",getenv("USER"));
        snprintf(runas,1000,"sudo -H -u %s bash -c 'xbindkeys'",getenv("USER"));

      }
      system(path);
      system("killall xbindkeys");
      system(runas);
      system("sudo systemctl restart systemd-logind");

      free(runas);
      free(path);

      printf("Your power button should now be set to open powermenu. If you experience any problems, please submit a bug report @ https://github.com/odinssecrets/powermenu/issues\n");   
      return 0;
    }
  }

  //GTK App stuff
  GtkApplication *app;
  int status;

  app = gtk_application_new ("org.gtk.power_menu", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc - opt_count, argv);
  g_object_unref (app);

  return status;
}

