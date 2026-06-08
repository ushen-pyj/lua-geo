local geo = require "geo"

-- cell_at: basic conversion
assert(geo.cell_at(0, 0, 32, 32) == 1)
assert(geo.cell_at(32, 0, 32, 32) == 2)
assert(geo.cell_at(0, 32, 32, 32) == 33)
assert(geo.cell_at(63, 63, 32, 32) == 34)

-- cell_origin: roundtrip consistency
local origin = geo.cell_origin(1, 32, 32)
assert(origin.x == 0 and origin.y == 0)
origin = geo.cell_origin(33, 32, 32)
assert(origin.x == 0 and origin.y == 32)
for _, idx in ipairs({1, 2, 33, 100, 1024}) do
    local o = geo.cell_origin(idx, 32, 32)
    assert(geo.cell_at(o.x, o.y, 32, 32) == idx)
end

-- dist
local d = geo.dist(0, 0, 3, 4)
assert(d == 5.0)
local d2 = geo.dist(0, 0, 100, 0)
assert(d2 == 100.0)

-- line_cells: horizontal line 0→100, cell_size=32
local result = geo.line_cells(0, 0, 100, 0, 10, 32, 0, 32)
assert(#result.cells >= 3)
assert(result.cells[1].idx > 0)
assert(result.cells[1].enter < result.cells[1].leave)
assert(result.arrive_time > 0)

-- enter/leave continuity
for i = 1, #result.cells - 1 do
    assert(result.cells[i].leave == result.cells[i + 1].enter)
end

-- zero distance: same start/end
local same = geo.line_cells(10, 10, 10, 10, 5, 32, 100, 32)
assert(#same.cells == 1)
assert(same.cells[1].enter == 100)
assert(same.cells[1].leave == 100)
assert(same.arrive_time == 100)

-- rect_cells: 2x2
local cells = geo.rect_cells(0, 0, 63, 63, 32, 32)
assert(#cells == 4)

-- rect_cells: single cell
local c = geo.rect_cells(0, 0, 31, 31, 32, 32)
assert(#c == 1)

print("All tests passed!")
