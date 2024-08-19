#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

double evaluate_expression(const char *expression) {
    double stack[128];
    double current_number = 0.0;
    int stack_index = 0;
    char operator = '+';

    const char *current = expression;
    while (*current) {
        while (isspace(*current)) {
            current++;
        }

        if (isdigit(*current) || *current == '.') {
            current_number = strtod(current, (char**)&current);
        }

        if (*current == '+' || *current == '-' || *current == '*' || *current == '/' || *current == '\0') {
            if (operator == '+' || operator == '-') {
                stack[stack_index++] = (operator == '+') ? current_number : -current_number;
            } else if (operator == '*') {
                stack[stack_index - 1] *= current_number;
            } else if (operator == '/') {
                if (current_number == 0.0) {
                    return 0.0;
                }
                stack[stack_index - 1] /= current_number;
            }

            operator = *current;
            current++;
            current_number = 0.0;
        }
    }

    double total = 0.0;
    for (int i = 0; i < stack_index; i++) {
        total += stack[i];
    }

    return total;
}

static void calculate_result(GtkEntry *entry) {
    const gchar *current_text = gtk_entry_get_text(entry);
    double result = evaluate_expression(current_text);
    gchar buffer[32];
    snprintf(buffer, sizeof(buffer), "%g", result);
    gtk_entry_set_text(entry, buffer);
}

static void on_button_clicked(GtkButton *button, gpointer user_data) {
    const gchar *button_label = gtk_button_get_label(button);
    GtkEntry *entry = GTK_ENTRY(user_data);

    if (strcmp(button_label, "=") == 0) {
        calculate_result(entry);
    } else {
        gchar *new_text = g_strdup_printf("%s%s", gtk_entry_get_text(entry), button_label);
        gtk_entry_set_text(entry, new_text);
        g_free(new_text);
    }
}

static void on_clear_clicked(GtkButton *button, gpointer user_data) {
    GtkEntry *entry = GTK_ENTRY(user_data);
    gtk_entry_set_text(entry, "");
}

static gboolean on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data) {
    if (event->keyval == GDK_KEY_Return || event->keyval == GDK_KEY_KP_Enter) {
        GtkEntry *entry = GTK_ENTRY(user_data);
        calculate_result(entry);
        return TRUE;
    }
    return FALSE;
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *entry;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Calculator");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);

    entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry, 0, 0, 4, 1);

    g_signal_connect(entry, "key-press-event", G_CALLBACK(on_key_press), entry);

    const gchar *buttons[16] = {
        "7", "8", "9", "/",
        "4", "5", "6", "*",
        "1", "2", "3", "-",
        "0", "C", "=", "+"
    };

    for (int i = 0; i < 16; i++) {
        GtkWidget *button = gtk_button_new_with_label(buttons[i]);
        g_signal_connect(button, "clicked", G_CALLBACK((strcmp(buttons[i], "C") == 0) ? on_clear_clicked : on_button_clicked), entry);
        gtk_grid_attach(GTK_GRID(grid), button, i % 4, (i / 4) + 1, 1, 1);
        gtk_widget_set_vexpand(button, TRUE);
        gtk_widget_set_hexpand(button, TRUE);
    }

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
