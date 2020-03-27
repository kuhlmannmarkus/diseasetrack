cc_binary(
    name = "Test_Crypto_App",
    srcs = ["test_crypto.cpp"],
    deps = [
        "//log:log",
	"//crypto:crypto",

    ],
    linkopts = ["-lsodium", "-lcrypto++"],
)
