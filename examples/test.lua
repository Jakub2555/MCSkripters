local ffi = require("ffi")

ffi.cdef[[
int __stdcall MessageBoxA(void *w, const char *txt, const char *cap, int type);
]]

function onInit()
  ffi.C.MessageBoxA(nil, "Init!", "Test", 0)
end

function onRender()
  render.drawFilledTriangle(200, 0, 0, 200, 200, 200, 0xFF00FFFF)
  
  render.drawText("MCSkripters", 60, 10, 10, 0xFFFFFFFF)
  
  render.drawCircle(500, 500, 150, 20, 0xFF00FFFF)
end

function onUpdate()
end

function onStop()
  ffi.C.MessageBoxA(nil, "Stop!", "Test", 0)
end