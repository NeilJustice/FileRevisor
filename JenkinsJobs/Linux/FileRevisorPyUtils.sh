#!/bin/bash

unset PYTHONPATH
python PyUtils/CoverageLintInstallPythonLibrary.py \
   --project=PyUtils \
   --test-project=PyUtilsTests \
   --run-tests-with-coverage-python-file=RunAllWithCoverage.py
