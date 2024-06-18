# iv_visualizer backend
This is the backend for the iv visualizer project. The accompanying frontend can be found at
https://gitlab.cc-asp.fraunhofer.de/cormier_students/iv/iv_visualizer_frontend

## Prerequisites
Before you can start working with the project, make sure you have the following prerequisites set up on your system:

1. **Docker Compose**: Docker Compose must be installed on your machine. You can download and install it from the [official Docker website](https://docs.docker.com/compose/install/).

2. **Log in to GitLab Container Registry**:
You can log in using the `docker login` command, providing your username and password:
    ```bash
    docker login registry.gitlab.cc-asp.fraunhofer.de
    ```

3. **Visual Studio Code**: Make sure you have Visual Studio Code installed on your system. Additionally, ensure that the [Dev Containers](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-containers) extension is also installed.


**Additionally**, you should have created a Docker network with:
```bash
docker network create --driver bridge ivNet
```

## How to build project
```bash
git clone https://gitlab.cc-asp.fraunhofer.de/cormier_students/iv/iv_visualizer.git
```
Open the project in VSCode and click on "Reopen in Container." Then, wait until the container has started.\
Open a new terminal in VSCode and run ``./build.sh``

## How to run code (after build and within build folder)
``./ivbackend``

## Run tests (after build and within build folder)
``./test/IV_VISUALIZER_test``

## How to generate documentation
``doxygen Doxyfile``
View docs in any modern browser

## How to instal hooks
- chmod+x scripts/move_hooks.sh
- ./move_hooks.sh
Hook will check code for proper formatting before it allows the commit. If formatting is wrong, it will generate a prompt which can be entered to format all un-formatted code according to our rules
After that just add and commit again
