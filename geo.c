#include "geo.h"
#include <math.h>

int
geo_cell_at(int x, int y, int cell_size, int cells_per_row)
{
    return (y / cell_size) * cells_per_row + (x / cell_size) + 1;
}

void
geo_cell_origin(int idx, int cell_size, int cells_per_row, int *ox, int *oy)
{
    int r = (idx - 1) / cells_per_row;
    int c = (idx - 1) % cells_per_row;
    *ox = c * cell_size;
    *oy = r * cell_size;
}

double
geo_dist(double x1, double y1, double x2, double y2)
{
    double dx = x2 - x1;
    double dy = y2 - y1;
    return sqrt(dx * dx + dy * dy);
}

struct geo_line_result *
geo_line_cells(double x1, double y1, double x2, double y2,
               double speed, int cell_size, int cells_per_row,
               int64_t start_time)
{
    struct geo_line_result *r = malloc(sizeof(*r));
    if (r == NULL) return NULL;

    int cap = 16;
    r->cells = malloc((size_t)cap * sizeof(struct geo_cell_entry));
    if (r->cells == NULL) {
        free(r);
        return NULL;
    }
    r->count = 0;

    double dx = x2 - x1;
    double dy = y2 - y1;
    double dist = sqrt(dx * dx + dy * dy);
    double duration = (dist / speed) * 100.0;
    int steps = (int)ceil(dist);

    int last = -1;
    for (int i = 0; i <= steps; i++) {
        double t = (steps == 0) ? 0.0 : (double)i / (double)steps;
        int x = (int)(x1 + dx * t);
        int y = (int)(y1 + dy * t);
        int cur = geo_cell_at(x, y, cell_size, cells_per_row);

        int64_t time = start_time + (int64_t)(duration * t);

        if (cur != last) {
            if (last != -1)
                r->cells[r->count - 1].leave = time;

            if (r->count >= cap) {
                cap *= 2;
                struct geo_cell_entry *tmp = realloc(r->cells,
                    (size_t)cap * sizeof(struct geo_cell_entry));
                if (tmp == NULL) {
                    geo_line_result_free(r);
                    return NULL;
                }
                r->cells = tmp;
            }

            r->cells[r->count].idx = cur;
            r->cells[r->count].enter = time;
            r->cells[r->count].leave = 0;
            r->count++;
            last = cur;
        }
    }

    if (r->count > 0)
        r->cells[r->count - 1].leave = start_time + (int64_t)duration;

    r->arrive_time = start_time + (int64_t)duration;
    return r;
}

struct geo_rect_result *
geo_rect_cells(int x1, int y1, int x2, int y2,
               int cell_size, int cells_per_row)
{
    int cx1 = x1 / cell_size;
    int cy1 = y1 / cell_size;
    int cx2 = x2 / cell_size;
    int cy2 = y2 / cell_size;

    int ncols = cx2 - cx1 + 1;
    int nrows = cy2 - cy1 + 1;
    int total = ncols * nrows;

    struct geo_rect_result *r = malloc(sizeof(*r));
    if (r == NULL) return NULL;

    r->cells = malloc((size_t)total * sizeof(int));
    if (r->cells == NULL) {
        free(r);
        return NULL;
    }
    r->count = total;

    int idx = 0;
    for (int cy = cy1; cy <= cy2; cy++)
        for (int cx = cx1; cx <= cx2; cx++)
            r->cells[idx++] = cy * cells_per_row + cx + 1;

    return r;
}

void
geo_line_result_free(struct geo_line_result *r)
{
    if (r == NULL) return;
    free(r->cells);
    free(r);
}

void
geo_rect_result_free(struct geo_rect_result *r)
{
    if (r == NULL) return;
    free(r->cells);
    free(r);
}
