#include <gtk/gtk.h>

static void on_button_clicked(GtkButton *button, gpointer data) {
    static int count = 0;
    count++;
    gchar *label = g_strdup_printf("Clicked %d times", count);
    gtk_button_set_label(button, label);
    g_free(label);
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *button;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Button Click Counter");
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 100);

    button = gtk_button_new_with_label("Click me!");
    gtk_container_add(GTK_CONTAINER(window), button);

    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), NULL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
