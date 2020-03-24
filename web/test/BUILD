cc_binary(
    name = "Test_WC_App",
    srcs = ["test_wc.cpp"],
    deps = [
        "//log:log",
        "//web:web",
    ],
    linkopts = ["-pthread","-lboost_program_options","-lboost_system","-lssl","-lcrypto","-lboost_filesystem","-ldl","-lboost_iostreams","-lsodium"],
)
