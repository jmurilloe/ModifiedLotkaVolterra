# Use Ubuntu 24.04 as base image
FROM ubuntu:24.04

# Avoid interactive prompts during installation
ENV DEBIAN_FRONTEND=noninteractive

# Update package list and install dependencies
RUN apt update && apt install -y \
    build-essential \
    libeigen3-dev \
    cmake \
    pkg-config \
    && rm -rf /var/lib/apt/lists/*

# # Updating gcc/g++
RUN apt-get update
RUN apt install software-properties-common -y
RUN apt install g++-14 -y

# # Installing starship: https://starship.rs/guide/
RUN apt install curl
RUN cd /tmp && curl -sS https://starship.rs/install.sh > install_starship.sh  &&  sh install_starship.sh --yes
RUN echo 'eval "$(starship init bash)"' >> ~/.bashrc

RUN apt install r-base -y

RUN R
RUN install.packages('bipartite', repos = 'https://www.icesi.edu.co/CRAN/')
RUN install.packages('Rcpp', repos = 'https://www.icesi.edu.co/CRAN/')
RUN install.packages('RInside', repos = 'https://www.icesi.edu.co/CRAN/')
RUN quit()
RUN y

# Set working directory
WORKDIR /workspace

# Default command
CMD ["/bin/bash"]