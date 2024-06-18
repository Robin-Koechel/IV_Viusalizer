# iv_visualizer_frontend

# How to build project
- ./build.sh

# How to run code (after build and within build folder)
- ./source/IV_VISUALIZER_run

# Run tests (after build and within build folder)
- ./test/IV_VISUALIZER_test

# How to generate documentation 
- sudo apt-get install doxygen 
- doxygen Doxyfile
View docs in any modern browser

# How to install hooks
- chmod+x scripts/move_hooks.sh
- ./move_hooks.sh
Hook will check code for proper formatting before it allows the commit. If formatting is wrong, it will generate a prompt which can be entered to format all un-formatted code according to our rules
After that just add and commit again