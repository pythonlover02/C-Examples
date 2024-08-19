#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h> // Include string.h for memcpy

#define WIDTH  800
#define HEIGHT 600
#define CELL_SIZE 10
#define GRID_WIDTH (WIDTH / CELL_SIZE)
#define GRID_HEIGHT (HEIGHT / CELL_SIZE)

static gboolean grid[GRID_WIDTH][GRID_HEIGHT];
static gboolean new_grid[GRID_WIDTH][GRID_HEIGHT];
static gint timer_interval = 100; // milliseconds

// Initialize grid with random values for demo purposes
void initialize_grid() {
    srand(time(NULL));
    for (int x = 0; x < GRID_WIDTH; x++) {
        for (int y = 0; y < GRID_HEIGHT; y++) {
            grid[x][y] = rand() % 2;
        }
    }
}

void update_grid(void) {
    int x, y, i, j, count;
    for (x = 0; x < GRID_WIDTH; x++) {
        for (y = 0; y < GRID_HEIGHT; y++) {
            count = 0;
            for (i = -1; i <= 1; i++) {
                for (j = -1; j <= 1; j++) {
                    if (i == 0 && j == 0) continue;
                    int nx = (x + i + GRID_WIDTH) % GRID_WIDTH;
                    int ny = (y + j + GRID_HEIGHT) % GRID_HEIGHT;
                    if (grid[nx][ny]) count++;
                }
            }
            new_grid[x][y] = (grid[x][y] && (count == 2 || count == 3)) || (!grid[x][y] && count == 3);
        }
    }
    memcpy(grid, new_grid, sizeof(grid));
}

gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer data) {
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_paint(cr);

    cairo_set_source_rgb(cr, 0, 0, 0);
    for (int x = 0; x < GRID_WIDTH; x++) {
        for (int y = 0; y < GRID_HEIGHT; y++) {
            if (grid[x][y]) {
                cairo_rectangle(cr, x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE);
                cairo_fill(cr);
            }
        }
    }
    return FALSE;
}

gboolean on_timeout(gpointer data) {
    update_grid();
    gtk_widget_queue_draw(GTK_WIDGET(data));
    return TRUE;
}

void on_button_press_event(GtkWidget *widget, GdkEventButton *event, gpointer data) {
    int x = event->x / CELL_SIZE;
    int y = event->y / CELL_SIZE;
    grid[x][y] = !grid[x][y];
    gtk_widget_queue_draw(widget);
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *drawing_area;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Conway's Game of Life");
    gtk_window_set_default_size(GTK_WINDOW(window), WIDTH, HEIGHT);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(drawing_area, WIDTH, HEIGHT);
    gtk_container_add(GTK_CONTAINER(window), drawing_area);
    g_signal_connect(drawing_area, "draw", G_CALLBACK(on_draw_event), NULL);
    g_signal_connect(drawing_area, "button-press-event", G_CALLBACK(on_button_press_event), NULL);

    g_timeout_add(timer_interval, on_timeout, drawing_area);

    gtk_widget_add_events(drawing_area, GDK_BUTTON_PRESS_MASK);
    gtk_widget_show_all(window);

    initialize_grid();

    gtk_main();

    return 0;
}

