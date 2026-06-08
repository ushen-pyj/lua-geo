---@meta geo

---@class geo.cell_entry
---@field idx     integer
---@field enter    integer
---@field leave    integer

---@class geo.line_result
---@field cells       geo.cell_entry[]
---@field arrive_time integer

---@param x integer
---@param y integer
---@param cell_size integer
---@param cells_per_row integer
---@return integer
function geo.cell_at(x, y, cell_size, cells_per_row) end

---@param idx integer
---@param cell_size integer
---@param cells_per_row integer
---@return { x: integer, y: integer }
function geo.cell_origin(idx, cell_size, cells_per_row) end

---@param x1 number
---@param y1 number
---@param x2 number
---@param y2 number
---@return number
function geo.dist(x1, y1, x2, y2) end

---@param x1 number
---@param y1 number
---@param x2 number
---@param y2 number
---@param speed number
---@param cell_size integer
---@param start_time integer
---@param cells_per_row integer
---@return geo.line_result
function geo.line_cells(x1, y1, x2, y2, speed, cell_size, start_time, cells_per_row) end

---@param x1 integer
---@param y1 integer
---@param x2 integer
---@param y2 integer
---@param cell_size integer
---@param cells_per_row integer
---@return integer[]
function geo.rect_cells(x1, y1, x2, y2, cell_size, cells_per_row) end
