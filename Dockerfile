FROM ubuntu:20.04

RUN apt-get -y update \
    && apt-get -y install sudo \
    && apt-get -y install nano \
    && apt-get install -y build-essential procps curl file git cmake 

RUN /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

WORKDIR /usr/src/app


CMD ["tail", "-f", "/dev/null"]