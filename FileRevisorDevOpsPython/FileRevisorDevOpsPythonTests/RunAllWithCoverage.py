import sys
sys.path.append('.')
from FileRevisorDevOpsPython import Python

Python.run_all_tests_with_coverage(testsProjectName='FileRevisorDevOpsPythonTests', omitPattern='/usr/*,/home/*/.local/lib/python3.9/site-packages/*')
