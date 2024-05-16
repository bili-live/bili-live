add_rules("mode.debug", "mode.release")

set_project("bili-live")
set_version("0.1.0", {build = "%Y%m%d%H%M"})

add_requires("imgui", {configs = {win32 = true, dx11 = true}})
add_requires("ffmpeg", {configs = {shared = false, ffmpeg = false, d3d11va = true, user_config = {
    "--disable-everything",
    "--enable-decoder=h264,hevc,aac,aac_fixed",
    "--enable-parser=h264,hevc,aac",
    "--enable-demuxer=flv,mpegts",
    "--enable-muxer=flv,mpegts"
}}})

target("bililive")
    set_default(true)
    set_basename("blive")
    set_kind("binary")
    add_files("src/**.cpp")
    add_packages("imgui")
    add_includedirs("src")
    add_defines("IMGUI_USE_WIN32", "IMGUI_USE_DX11")
    add_syslinks("d3d11")