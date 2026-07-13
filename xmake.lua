add_rules("mode.debug", "mode.release")
add_rules("plugin.vsxmake.autoupdate")
add_rules("plugin.compile_commands.autoupdate")

add_repositories("levimc-repo https://github.com/LiteLDev/xmake-repo.git")

-- add_requires("levilamina 26.10.*", { configs = { target_type = "client" } })
add_requires(
    "entt v3.15.0",
    "expected-lite v0.8.0",
    "fmt 11.2.0",
    "gsl v4.2.0",
    "glm 1.0.1",
    "leveldb 1.23",
    "magic_enum v0.9.7",
    "nlohmann_json v3.11.3",
    "rapidjson v1.1.0",
    "type_safe v0.2.4",
    "pcg_cpp v1.0.0",
    "pfr 2.1.1",
    "symbolprovider v1.2.0",
    "parallel-hashmap v1.3.12",
    "concurrentqueue v1.0.4",
    "stb 2025.03.14",
    "bedrockdata v26.10.4-client.17"
)
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
    -- add_packages(
    --     "levilamina"
    -- )
    add_packages(
        "entt",
        "expected-lite",
        "fmt",
        "gsl",
        "glm",
        "leveldb",
        "magic_enum",
        "nlohmann_json",
        "rapidjson",
        "type_safe",
        "pcg_cpp",
        "pfr",
        "symbolprovider",
        "parallel-hashmap",
        "concurrentqueue",
        "stb",
        "bedrockdata"
    )
    set_exceptions("none")
    set_kind("shared")
    set_languages("c++20")
    set_symbols("debug")
    add_headerfiles("src/**.h")
    add_files("src/**.cpp")
    add_includedirs("src")
    set_optimize("aggressive")

    -- sdk
    add_includedirs("sdk/include")
    add_headerfiles("sdk/include/**.h")
    add_links("sdk/lib/LeviLamina.lib")