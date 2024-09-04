#!/usr/bin/env python3

import sys
import csv
import argparse
import xml.etree.ElementTree as ET
from datetime import datetime
from pathlib import Path

# Color codes
RED = '\033[91m'
GREEN = '\033[92m'
BLUE = '\033[94m'
RESET = '\033[0m'


def load_baseline(file_path):
    """Load the baseline data from the specified CSV file."""
    baseline_data = {}
    with open(file_path, mode='r') as csvfile:
        reader = csv.DictReader(csvfile)
        for row in reader:
            test_name = row["Test Name"]
            expected_time = float(row["Expected Execution Time"])  # Correct column name
            baseline_data[test_name] = expected_time
    return baseline_data


def parse_gtest_xml(xml_path):
    results = {}
    test_suites = {}
    tree = ET.parse(xml_path)
    root = tree.getroot()

    for testsuite in root.findall(".//testsuite"):
        suite_name = testsuite.get("name")
        test_suites[suite_name] = []
        for testcase in testsuite.findall("testcase"):
            test_name = testcase.get("name")
            time = float(testcase.get("time", 0))
            results[test_name] = (time, suite_name)
            test_suites[suite_name].append(test_name)

    return results, test_suites


def compare_results(baseline_data, results, baseline_filename):
    comparison_results = []
    test_suites_reported = set()

    for test_name, (new_time, suite_name) in results.items():
        if suite_name not in test_suites_reported:
            if test_suites_reported:
                comparison_results.append(f"\n{RESET}")
            test_suites_reported.add(suite_name)
            comparison_results.append(f"\n{BLUE}\033[1mTest Suite: {suite_name}\033[0m")

        expected_time = baseline_data.get(test_name)
        if expected_time is not None:
            if new_time == expected_time:
                comparison_results.append(
                    f"Comparing {test_name} against {baseline_filename}: Expected Time = {expected_time}, New Time = {new_time} {GREEN}(NICE){RESET}")
            else:
                comparison_results.append(
                    f"Comparing {test_name} against {baseline_filename}: Expected Time = {expected_time}, New Time = {new_time} {RED}(REGRESSION){RESET}")
        else:
            comparison_results.append(f"Test {test_name} not found in {baseline_filename} {RED}(REGRESSION){RESET}")

    return comparison_results


def write_results_to_csv(results_file, results, baseline_data, append=False):
    timestamp = datetime.now().strftime('%Y-%m-%dT%H:%M:%S.%f')[:-3]  # Current timestamp with milliseconds

    file_mode = 'a' if append else 'w'
    write_header = not append or not Path(results_file).exists() or Path(results_file).stat().st_size == 0

    with open(results_file, mode=file_mode, newline='') as csvfile:
        fieldnames = ['Test Suite', 'Test Name', 'Execution Time', 'Timestamp', 'Result', 'Regression']
        writer = csv.DictWriter(csvfile, fieldnames=fieldnames)

        if write_header:
            writer.writeheader()

        # Write a blank line to separate batches if appending
        if append:
            csvfile.write('\n')

        for test_name, (exec_time, suite_name) in results.items():
            result = 'completed'
            regression = ''
            if test_name not in baseline_data:
                regression = 'REGRESSION'
            else:
                expected_time = baseline_data[test_name]
                if exec_time != expected_time:
                    regression = 'REGRESSION'

            writer.writerow({
                'Test Suite': suite_name,
                'Test Name': test_name,
                'Execution Time': exec_time,
                'Timestamp': timestamp,
                'Result': result,
                'Regression': regression
            })


def main():
    parser = argparse.ArgumentParser(description='Process and compare gtest XML output with a baseline.')
    parser.add_argument('baseline_file', type=str, help='Path to the baseline CSV file')
    parser.add_argument('results_file', type=str, help='Path to the gtest XML output file')
    parser.add_argument('output_csv', type=str, help='Path to the output results CSV file')

    args = parser.parse_args()

    baseline_file = args.baseline_file
    results_file = args.results_file
    output_csv = args.output_csv

    # Extract the baseline filename without extension
    baseline_filename = Path(baseline_file).stem

    try:
        baseline_data = load_baseline(baseline_file)
    except Exception as e:
        print(f"{RED}Error loading baseline file: {e}{RESET}")
        sys.exit(1)

    try:
        results, test_suites = parse_gtest_xml(results_file)
    except Exception as e:
        print(f"{RED}Error parsing results XML: {e}{RESET}")
        sys.exit(1)

    comparison_results = compare_results(baseline_data, results, baseline_filename)
    for line in comparison_results:
        print(line)

    append_data = input("Do you want to append the results to the existing file? (y/n): ").strip().lower()
    if append_data == 'y':
        write_results_to_csv(output_csv, results, baseline_data, append=True)


if __name__ == "__main__":
    if len(sys.argv) != 4:
        print(f"{RED}Usage: {sys.argv[0]} <baseline_file> <results_file> <output_csv>{RESET}")
        sys.exit(1)

    main()
