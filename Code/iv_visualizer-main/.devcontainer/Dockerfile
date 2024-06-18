FROM registry.gitlab.cc-asp.fraunhofer.de/cormier_students/docker-images/ubuntu_22.04:iv.visualizer.dev
USER root
# To avoid waiting for input during package installation
ENV DEBIAN_FRONTEND=noninteractive
ENV QT_QPA_PLATFORM="xcb"
RUN pip install pre-commit
RUN apt-get update && apt-get install -y \
    libcairo2-dev \
    libx264-dev \
    librsvg2-dev \
    libgdk-pixbuf2.0-dev

