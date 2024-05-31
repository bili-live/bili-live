includes("script/packages.lua")

add_rules("mode.debug", "mode.release")

set_project("bili-live")
set_version("0.1.0", {build = "%Y%m%d%H%M"})
set_languages("cxx20") -- c++20

add_requires("rapidjson") -- json
add_requires("async_simple") -- cxx 20 coroutine
add_requires("mongoose") -- http/websocket support
add_requires("imgui", {configs = {shared = false, win32 = true, dx11 = true}}) -- gui
add_requires("ffmpeg", {configs = {
    shared = false, ffmpeg = false, d3d11va = true,
    user_config = {
        "--disable-everything",
        "--enable-decoder=h264,hevc,aac,aac_fixed",
        "--enable-parser=h264,hevc,aac",
        "--enable-demuxer=flv,mpegts",
        "--enable-muxer=flv,mpegts",
        "--disable-network",
        "--disable-postproc",
        "--disable-avdevice",
        "--disable-d3d12va"
    }
}}) -- video decode

target("bililive")
    set_default(true)
    set_basename("blive")
    set_kind("binary")
    add_files("src/**.cpp")
    add_packages("imgui")
    add_includedirs("src")
    add_defines("IMGUI_USE_WIN32", "IMGUI_USE_DX11")
    add_syslinks("d3d11")