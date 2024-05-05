#!/bin/sh

set_project "bili-live"
set_version "0.0.1" "%Y%m%d%H%M"

# ffmpeg options begin
## linux hardware decode support
option "vaapi" "enable vaapi" false
option "vdpau" "enable vdpau" false
## windows hardware decode support
option "d3d11va" "enable d3d11va" true
option "dxva2" "enable dxva2" false
option "mediafoundation" "enable mediafoundation" true
## hardcoded-tables support
option "hardcoded-tables" "enable hardcoded-tables" false
# ffmpeg options end

# imgui backend
option "imgui-plat" "imgui platform" "win32"
option "imgui-backend" "imgui backend" "dx11"

set_warnings "all" "error"
set_languages "c99" "c++17"

if is_mode "debug"; then
    set_symbols "debug"
    set_optimize "none"
else
    set_strip "all"
    set_symbols "hidden"
    set_optimize "smallest"
fi

add_includedirs "lib/imgui" "lib/imgui/backends"
target "imgui"
    set_kind "static"
    add_files "lib/imgui/imgui*.cpp"
    add_files "lib/imgui/backends/imgui_impl_$(get_config "imgui-backend").cpp"
    add_files "lib/imgui/backends/imgui_impl_$(get_config "imgui-plat").cpp"

target "bililive"
    set_default true
    set_basename "blive"
    add_deps "imgui"
    set_kind "binary"
    add_files "src/*.cpp"
    if test_match "$(get_config "imgui-backend")" "dx*"; then
        add_syslinks "d3d11" "d3dcompiler" "gdi32" "dwmapi" "user32"
    fi