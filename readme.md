# Project Zeta firmware

## Initial setup for developers

1. Install vscode extensions

    platformIO is the only absolutely required extension in the list,
    however other extensions are recommended since it will make your life easier once you have it setup.\
    Note: platformIO will create some files/directories such as `.pio` which are ignored by git because
    they contain machine specific paths and gets auto-generated anyways.

2. Install pre-commit and configure it using `.pre-commit-condig.yaml`

    **TLDR**

    ```shell
    pyenv install 3.8.12
    pip install pre-commit
    pre-commit install
    ```

    You have to choose what python version management tool to use, but **pyenv** is recommended.
    This instruction is going to assume you are using pyenv and pip.\
    Once you have pyenv, install python version **3.8.12** by `pyenv install 3.8.12`.\
    Then install **pre-commit** by `pip install pre-commit`, and configure it by `pre-commit install`.

## Contributors

Roy Ataya, Aidan Cook, Hamza Kamal, Kirill Melnikov, Paige Rattenberry, and Aki Zhou
