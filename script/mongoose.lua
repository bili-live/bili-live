add_rules("mode.debug", "mode.release")

option("ssl")
    set_default("builtin")
    set_showmenu(true)
    set_values("builtin", "openssl", "mbedtls")
option_end()
if has_config("ssl") then
    if get_config("ssl") == "openssl" then
        add_requires("openssl")
    elseif get_config("ssl") == "mbedtls" then
        add_requires("mbedtls")
    end
end

option("io_size")
    set_default(8192)
    set_showmenu(true)
option_end()

option("ssi")
    set_default(false)
    set_showmenu(true)
option_end()

target("mongoose")
    set_kind("static")
    add_files("mongoose.c")
    add_includedirs(".", {public = true})
    if get_config("ssl") == "builtin" then
        add_defines("MG_TLS=MG_TLS_BUILTIN")
    elseif get_config("ssl") == "openssl" then
        add_defines("MG_TLS=MG_TLS_OPENSSL")
    elseif get_config("ssl") == "mbedtls" then
        add_defines("MG_TLS=MG_TLS_MBEDTLS")
    end
    if get_config("ssi") then
        add_defines("MG_ENABLE_SSI=1")
    else 
        add_defines("MG_ENABLE_SSI=0")
    end
    add_defines("MG_IO_SIZE=$(io_size)")