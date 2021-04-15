import sys
sys.path.append('.')
from FileRevisorPyUtils import Python

Python.run_all_tests_with_coverage(testsProjectName='FileRevisorPyUtilsTests', omitPattern='/usr/*,/home/*/.local/lib/python3.9/site-packages/*')