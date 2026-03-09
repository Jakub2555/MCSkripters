local ffi = require("ffi")

ffi.cdef[[
int __stdcall MessageBoxA(void *w, const char *txt, const char *cap, int type);
]]

function onInit()
  ffi.C.MessageBoxA(nil, "Init!", "Test", 0)
end

function onRender()
  render.drawFilledRect(8, 8, 90, 32, 0, 0, 0, 1)
  render.drawText("MCSkripters", 10, 10, 1, 1, 1, 1)
end

function onUpdate()
end

function onStop()
  ffi.C.MessageBoxA(nil, "Stop!", "Test", 0)
end