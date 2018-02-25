#!/bin/bash

lcov --capture --directory . --output-file coverage.info
lcov --directory . --output-file coverage.info \
     --remove coverage.info "/usr/*" "*moc*" "*ui_*" "*/test/*"
genhtml coverage.info -t Presenter_Server -o coverage
