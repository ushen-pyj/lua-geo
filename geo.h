#pragma once

#include <stdint.h>
#include <stdlib.h>

struct geo_cell_entry {
    int idx;
    int64_t enter;
    int64_t leave;
};

struct geo_line_result {
    struct geo_cell_entry *cells;
    int count;
    int64_t arrive_time;
};

struct geo_rect_result {
    int *cells;
    int count;
};

int geo_cell_at(int x, int y, int cell_size, int cells_per_row);

void geo_cell_origin(int idx, int cell_size, int cells_per_row, int *ox, int *oy);

double geo_dist(double x1, double y1, double x2, double y2);

struct geo_line_result *geo_line_cells(double x1, double y1, double x2, double y2,
                                       double speed, int cell_size,
                                       int cells_per_row, int64_t start_time);

struct geo_rect_result *geo_rect_cells(int x1, int y1, int x2, int y2,
                                       int cell_size, int cells_per_row);

void geo_line_result_free(struct geo_line_result *r);
void geo_rect_result_free(struct geo_rect_result *r);
