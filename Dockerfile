FROM espressif/idf:release-v5.3

ENV DEBIAN_FRONTEND=noninteractive
ENV HOME=/home/dev

# Install required packages for Development
RUN apt-get update && apt-get install -y --no-install-recommends \
    vim \
    git \
    curl \
    ca-certificates \
    bash-completion \
    cscope \
    exuberant-ctags \
    global \
    ripgrep \
    fd-find \
    build-essential \
    python3 \
    python3-pip \
    fzf \
    && rm -rf /var/lib/apt/lists/*

RUN ln -s /usr/bin/fdfind /usr/local/bin/fd || true

RUN mkdir -p /home/dev && chmod 777 /home/dev

WORKDIR /work

# ESP-IDF auto env
RUN echo 'export HOME=/home/dev' >> /etc/bash.bashrc
RUN echo 'source $IDF_PATH/export.sh >/dev/null 2>&1' >> /etc/bash.bashrc
RUN echo 'PS1="[esp32] \\w $ "' >> /etc/bash.bashrc

SHELL ["/bin/bash", "-c"]
