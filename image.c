#include <gtk/gtk.h>

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *image;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Simple Image Viewer");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 300);

    image = gtk_image_new_from_file("example.jpg");
    gtk_container_add(GTK_CONTAINER(window), image);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
