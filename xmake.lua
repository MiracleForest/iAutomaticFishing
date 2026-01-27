add_rules("mode.debug", "mode.release")
add_rules("plugin.vsxmake.autoupdate")
add_rules("plugin.compile_commands.autoupdate")

add_repositories("levimc-repo https://github.com/LiteLDev/xmake-repo.git")

add_requires("levilamina 1.9.2", { configs = { target_type = "client" } })
add_requires("levibuildscript 0.5.2")

if not has_config("vs_runtime") then
    set_runtimes("MD")
end

target("iAutomaticFishing")
    add_rules("@levibuildscript/linkrule")
    add_rules("@levibuildscript/modpacker")
    add_cxflags(
        "/EHa",
        "/utf-8",
        "/W4",
        "/w44265",
        "/w44289",
        "/w44296",
        "/w45263",
        "/w44738",
        "/w45204"
    )
    add_defines(
        "NOMINMAX",
        "UNICODE",
        "_HAS_CXX23=1",
        "LL_PLAT_C"
    )
    add_packages(
        "levilamina"
    )
    set_exceptions("none")
    set_kind("shared")
    set_languages("c++20")
    set_symbols("debug")
    add_headerfiles("src/**.h")
    add_files("src/**.cpp")
    add_includedirs("src")
    set_optimize("fastest")
    