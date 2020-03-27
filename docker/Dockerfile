FROM debian:buster-slim
RUN apt update
RUN apt install apt-transport-https ca-certificates gnupg pkg-config zip g++ zlib1g-dev unzip python3 curl libboost-dev libboost-program-options-dev libboost-iostreams-dev libboost-system-dev libboost-filesystem-dev git libssl-dev libsodium-dev libcrypto++-dev -y && echo "deb [arch=amd64] https://storage.googleapis.com/bazel-apt stable jdk1.8" | tee /etc/apt/sources.list.d/bazel.list && curl https://bazel.build/bazel-release.pub.gpg | apt-key add - && apt update && apt install bazel -y
RUN git clone https://github.com/kuhlmannmarkus/diseasetrack.git && cd diseasetrack && git rev-parse HEAD && cat api/api.cpp && bazel build //...
VOLUME ["/diseasetrack"]
COPY start.sh /start.sh
CMD ["./start.sh"]
EXPOSE 12345
