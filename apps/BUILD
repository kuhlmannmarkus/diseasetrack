cc_binary(
    name = "DaemonApp",
    srcs = ["DaemonApp.cpp"],
    linkopts = ["-lboost_program_options","-lboost_system","-lssl","-lcrypto","-lboost_filesystem","-ldl","-pthread","-lboost_iostreams","-lsodium","-lcrypto++"],
    deps = ["//log:log",
            "//common:defaults",
	    "//web:web",
	    "//reaction:reaction",

    ],
)
