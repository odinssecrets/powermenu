#include <gtk/gtk.h> 
#include <stdlib.h>

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
  GdkScreen *screen = gdk_screen_get_default();
  
  GtkWidget *window;
  GtkWidget *grid = gtk_grid_new();

  GtkWidget *powerOffButton;
  GtkWidget *restartButton;
  GtkWidget *suspendButton;
  GtkWidget *cancelButton;
  
  GtkWidget *pob_box; //Power Off Button box
  GtkWidget *rb_box;  // '  '
  GtkWidget *sb_box;  // '  '
  GtkWidget *cb_box;  // '  '
  
  gint monitorWidth = gdk_screen_get_width(screen);
  gint monitorHeight = gdk_screen_get_height(screen);
  gint unit = monitorHeight/5;

  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Power");
  gtk_window_set_default_size (GTK_WINDOW (window), unit*4, unit);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER_ALWAYS);
  gtk_window_set_decorated(GTK_WINDOW(window), FALSE);
  gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

  GtkWidget *power_buf = gdk_pixbuf_new_from_file_at_size( "images/pwr.png", unit/2, unit/2, NULL); 
  GtkWidget *restart_buf = gdk_pixbuf_new_from_file_at_size( "images/restart.png", unit/2, unit/2, NULL);
  GtkWidget *suspend_buf = gdk_pixbuf_new_from_file_at_size( "images/suspend.png", unit/2, unit/2, NULL);
  GtkWidget *cancel_buf = gdk_pixbuf_new_from_file_at_size( "images/cancel.png", unit/2, unit/2, NULL);
  
  GtkWidget *powerImg   = gtk_image_new_from_pixbuf(power_buf);
  GtkWidget *restartImg = gtk_image_new_from_pixbuf(restart_buf);
  GtkWidget *suspendImg = gtk_image_new_from_pixbuf(suspend_buf); 
  GtkWidget *cancelImg  = gtk_image_new_from_pixbuf(cancel_buf);
 
  //GTK Buttons always show images
  GtkSettings *settings = gtk_settings_get_default();
  g_object_set(settings, "gtk-button-images", TRUE, NULL); 

  gtk_grid_set_row_homogeneous ( grid, TRUE );
  gtk_grid_set_column_homogeneous ( grid, TRUE );
  
  gtk_container_add (GTK_CONTAINER (window), grid);
  
  pob_box = gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL);
  rb_box  = gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL);
  sb_box  = gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL);
  cb_box  = gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL);

  gtk_grid_attach ( grid, pob_box, 0, 0, 1, 1);
  gtk_grid_attach ( grid, rb_box,  1, 0, 1, 1);
  gtk_grid_attach ( grid, sb_box,  2, 0, 1, 1);
  gtk_grid_attach ( grid, cb_box,  3, 0, 1, 1);
  
  powerOffButton  = gtk_button_new_with_label ("Shut Down");
  restartButton   = gtk_button_new_with_label ("Restart");
  suspendButton   = gtk_button_new_with_label ("Suspend");
  cancelButton    = gtk_button_new_with_label ("Cancel");

  gtk_button_set_image( powerOffButton, powerImg    );
  gtk_button_set_image( restartButton,  restartImg  );
  gtk_button_set_image( suspendButton,  suspendImg  );
  gtk_button_set_image( cancelButton,   cancelImg   );
  
  gtk_button_set_image_position( powerOffButton,  GTK_POS_TOP );
  gtk_button_set_image_position( restartButton,   GTK_POS_TOP );
  gtk_button_set_image_position( suspendButton,   GTK_POS_TOP );
  gtk_button_set_image_position( cancelButton,    GTK_POS_TOP );
  
  gtk_widget_set_size_request( powerOffButton, (gint)unit/1.5, (gint)unit/1.5 );
  gtk_widget_set_size_request( restartButton, (gint)unit/1.5, (gint)unit/1.5 );
  gtk_widget_set_size_request( suspendButton, (gint)unit/1.5, (gint)unit/1.5 );
  gtk_widget_set_size_request( cancelButton, (gint)unit/1.5, (gint)unit/1.5 );

  g_signal_connect (powerOffButton, "clicked", G_CALLBACK (shutdown), NULL);
  g_signal_connect (restartButton,  "clicked",   G_CALLBACK (reboot), NULL);
  g_signal_connect (suspendButton,  "clicked",  G_CALLBACK (suspend), NULL);
  
  g_signal_connect_swapped (powerOffButton, "clicked", G_CALLBACK (gtk_widget_destroy), window);
  g_signal_connect_swapped (restartButton, "clicked", G_CALLBACK (gtk_widget_destroy), window);
  g_signal_connect_swapped (suspendButton, "clicked", G_CALLBACK (gtk_widget_destroy), window);
  g_signal_connect_swapped (cancelButton, "clicked", G_CALLBACK (gtk_widget_destroy), window);

  gtk_container_add (GTK_CONTAINER (pob_box), powerOffButton );
  gtk_container_add (GTK_CONTAINER (rb_box),  restartButton );
  gtk_container_add (GTK_CONTAINER (sb_box),  suspendButton );
  gtk_container_add (GTK_CONTAINER (cb_box),  cancelButton );
  
  gtk_widget_show_all (window);
}

int main (int argc,char **argv)
{
  GtkApplication *app;
  int status;

  app = gtk_application_new ("org.gtk.arch_power_menu", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}
