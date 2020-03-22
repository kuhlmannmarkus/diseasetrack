workspace(name = "domain_diseasetrack")

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository", "new_git_repository")

new_git_repository(
    name = "SigSlot",
    commit = "b968051e0ab4fd2532c7da907423a7203217b83a",
    remote = "https://gitlab.com/herrkpunkt/sigslot.git",
    build_file_content = """cc_library(name = "SigSlot", hdrs = glob(["sigslot.h"]), visibility = ["//visibility:public"])"""
)

new_git_repository(
    name = "SimpleWebServer",
    commit = "1bc0b9a779a1bd556d9ba12535f04ee5a1e1575c",
    remote = "https://gitlab.com/eidheim/Simple-Web-Server.git",
    build_file_content = """cc_library(name = "SimpleWebServer", hdrs = glob(["*.h","*.hpp"]), visibility = ["//visibility:public"])"""
)

new_git_repository(
    name = "cpp-base64",
    commit = "a8aae956a2f07df9aac25b064cf4cd92d56aac45",
    remote = "https://github.com/ReneNyffenegger/cpp-base64.git",
    build_file_content = """cc_library(name = "cpp-base64", hdrs = glob(["*.h"]), srcs = ["base64.cpp"], visibility = ["//visibility:public"])"""
)
