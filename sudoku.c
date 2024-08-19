#include <gtk/gtk.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 9

// Structure to hold the grid and its widgets
typedef struct {
    int grid[SIZE][SIZE];
    int solution[SIZE][SIZE];
    GtkWidget *labels[SIZE][SIZE];
    GtkWidget *reveal_button;
} SudokuData;

// Function to print the Sudoku grid (for debugging)
void print_grid(int grid[SIZE][SIZE]) {
    for (int r = 0; r < SIZE; r++) {
        for (int d = 0; d < SIZE; d++) {
            printf("%d ", grid[r][d]);
        }
        printf("\n");
    }
}

// Function to check if a number can be placed in a specific position
int is_safe(int grid[SIZE][SIZE], int row, int col, int num) {
    for (int x = 0; x < SIZE; x++) {
        if (grid[row][x] == num || grid[x][col] == num) {
            return 0;
        }
    }

    int startRow = row - row % 3, startCol = col - col % 3;
    for (int r = 0; r < 3; r++) {
        for (int d = 0; d < 3; d++) {
            if (grid[r + startRow][d + startCol] == num) {
                return 0;
            }
        }
    }
    return 1;
}

// Function to fill the Sudoku grid
int fill_grid(int grid[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (grid[i][j] == 0) {
                for (int num = 1; num <= SIZE; num++) {
                    if (is_safe(grid, i, j, num)) {
                        grid[i][j] = num;
                        if (fill_grid(grid)) {
                            return 1;
                        }
                        grid[i][j] = 0; // Backtrack
                    }
                }
                return 0; // Trigger backtracking
            }
        }
    }
    return 1;
}

// Function to generate a random Sudoku puzzle
void generate_sudoku(SudokuData *data) {
    // Clear the grid
    for (int r = 0; r < SIZE; r++)
        for (int d = 0; d < SIZE; d++)
            data->grid[r][d] = 0;

    fill_grid(data->grid);

    // Copy the solution to the data structure
    for (int r = 0; r < SIZE; r++)
        for (int d = 0; d < SIZE; d++)
            data->solution[r][d] = data->grid[r][d];

    // Remove some numbers to create the puzzle
    int count = 20; // Number of cells to remove
    while (count != 0) {
        int i = rand() % SIZE;
        int j = rand() % SIZE;
        if (data->grid[i][j] != 0) {
            data->grid[i][j] = 0;
            count--;
        }
    }
}

// Callback function for the "Reveal Answer" button
void on_reveal_clicked(GtkButton *button, gpointer user_data) {
    SudokuData *data = (SudokuData *)user_data;

    // Update the labels with the solution
    for (int r = 0; r < SIZE; r++) {
        for (int d = 0; d < SIZE; d++) {
            char *text = g_strdup_printf("%d", data->solution[r][d]);
            gtk_label_set_text(GTK_LABEL(data->labels[r][d]), text);
            g_free(text);
        }
    }

    // Disable the button after clicking
    gtk_widget_set_sensitive(data->reveal_button, FALSE);
}

// Function to create the GTK interface
void create_sudoku_window(SudokuData *data) {
    GtkWidget *window;
    GtkWidget *grid_widget;
    GtkWidget *button_box;

    gtk_init(NULL, NULL);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Random Sudoku");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 300);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create a vertical box to hold the grid and button
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    grid_widget = gtk_grid_new();
    gtk_box_pack_start(GTK_BOX(vbox), grid_widget, TRUE, TRUE, 0);

    for (int r = 0; r < SIZE; r++) {
        for (int d = 0; d < SIZE; d++) {
            data->labels[r][d] = gtk_label_new(data->grid[r][d] == 0 ? "" : g_strdup_printf("%d", data->grid[r][d]));
            gtk_label_set_width_chars(GTK_LABEL(data->labels[r][d]), 2);
            gtk_grid_attach(GTK_GRID(grid_widget), data->labels[r][d], d, r, 1, 1);
        }
    }

    // Create the reveal button
    data->reveal_button = gtk_button_new_with_label("Reveal Answer");
    g_signal_connect(data->reveal_button, "clicked", G_CALLBACK(on_reveal_clicked), data);
    gtk_box_pack_start(GTK_BOX(vbox), data->reveal_button, FALSE, FALSE, 0);

    gtk_widget_show_all(window);
    gtk_main();
}

// Main function
int main(int argc, char *argv[]) {
    srand(time(NULL)); // Seed for random number generation
    SudokuData data;

    generate_sudoku(&data);
    print_grid(data.grid); // Print the generated Sudoku to the console
    create_sudoku_window(&data);

    return 0;
}
