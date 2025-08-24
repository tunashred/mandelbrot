#!/usr/bin/env python3
"""
Test Performance Regression Analyzer

Compares two XML test result files and detects performance regressions.
Usage: python test_analyzer.py input.xml baseline.xml
"""

import sys
import xml.etree.ElementTree as ET
from typing import Dict, Tuple, Optional
from dataclasses import dataclass


class Colors:
    """ANSI color codes for terminal output"""
    RED = '\033[91m'
    GREEN = '\033[92m'
    YELLOW = '\033[93m'
    BLUE = '\033[94m'
    RESET = '\033[0m'
    BOLD = '\033[1m'


@dataclass
class TestResult:
    """Represents a single test result with its properties"""
    name: str
    classname: str
    testsuite_name: str
    time: float
    properties: Dict[str, str]

    def get_signature(self) -> str:
        """Generate a unique signature for matching tests"""
        # Sort properties for consistent signature
        prop_str = "_".join(f"{k}:{v}" for k, v in sorted(self.properties.items()))
        return f"{self.name}_{prop_str}"

    def get_full_name(self) -> str:
        """Get the full test name including classname for display purposes"""
        if self.classname:
            return f"{self.classname}.{self.name}"
        return self.name

    def get_class_path(self) -> str:
        """Get just the class path without test parameters for concise display"""
        if self.classname:
            return self.classname
        return self.name.split('/')[0] if '/' in self.name else self.name

    def get_data_type(self) -> str:
        """Extract data type from testsuite name"""
        if not self.testsuite_name:
            return ""

        # Look for common data type patterns at the end of testsuite name
        testsuite_parts = self.testsuite_name.split('/')
        if len(testsuite_parts) >= 2:
            class_part = testsuite_parts[-1]  # e.g., "AlignedMatrixSharedInt"

            # Common data types to look for
            data_types = ['Int', 'Double', 'Float', 'Long', 'Short', 'Char']
            for dtype in data_types:
                if class_part.endswith(dtype):
                    return dtype

        return ""

    def get_display_name(self) -> str:
        """Get test name with data type for display"""
        data_type = self.get_data_type()
        if data_type:
            return f"{self.name} ({data_type})"
        return self.name


class TestRegessionAnalyzer:
    def __init__(self, regression_threshold: float = 0.05):
        self.regression_threshold = regression_threshold

    def parse_xml_file(self, filepath: str) -> Dict[str, TestResult]:
        """Parse XML test file and return dictionary of test results"""
        try:
            tree = ET.parse(filepath)
            root = tree.getroot()
        except ET.ParseError as e:
            print(f"Error parsing XML file {filepath}: {e}")
            sys.exit(1)
        except FileNotFoundError:
            print(f"File not found: {filepath}")
            sys.exit(1)

        tests = {}

        # Find all testcase elements
        for testsuite in root.findall(".//testsuite"):
            testsuite_name = testsuite.get("name", "")

            for testcase in testsuite.findall("testcase"):
                name = testcase.get("name", "")
                classname = testcase.get("classname", "")
                time_str = testcase.get("time", "0")

                try:
                    time = float(time_str)
                except ValueError:
                    print(f"Warning: Invalid time value '{time_str}' for test {name}")
                    continue

                # Extract properties
                properties = {}
                properties_elem = testcase.find("properties")
                if properties_elem is not None:
                    for prop in properties_elem.findall("property"):
                        prop_name = prop.get("name", "")
                        prop_value = prop.get("value", "")
                        if prop_name:
                            properties[prop_name] = prop_value

                test_result = TestResult(name, classname, testsuite_name, time, properties)
                signature = test_result.get_signature()
                tests[signature] = test_result

        return tests

    def compare_tests(self, input_tests: Dict[str, TestResult],
                      baseline_tests: Dict[str, TestResult]) -> None:
        """Compare input tests against baseline and report regressions"""

        input_signatures = set(input_tests.keys())
        baseline_signatures = set(baseline_tests.keys())

        # Find tests that exist in both files
        common_tests = input_signatures & baseline_signatures

        # Report tests that don't match
        input_only = input_signatures - baseline_signatures
        baseline_only = baseline_signatures - input_signatures

        if input_only:
            print(f"Info: {len(input_only)} test(s) found only in input file:")
            for sig in sorted(input_only):
                test = input_tests[sig]
                print(f"    {test.get_full_name()} {test.properties}")
            print()

        if baseline_only:
            print(f"Info: {len(baseline_only)} test(s) found only in baseline file:")
            for sig in sorted(baseline_only):
                test = baseline_tests[sig]
                print(f"    {test.get_full_name()} {test.properties}")
            print()

        if not common_tests:
            print("No matching tests found between input and baseline files!")
            return

        print(f"Analyzing {len(common_tests)} matching test(s)...\n")

        # Analyze performance changes
        regressions = []
        improvements = []
        stable_tests = []

        for signature in sorted(common_tests):
            input_test = input_tests[signature]
            baseline_test = baseline_tests[signature]

            input_time = input_test.time
            baseline_time = baseline_test.time

            # Handle zero baseline time
            if baseline_time == 0:
                if input_time > 0:
                    print(
                        f"⚠️  Warning: {input_test.get_full_name()} has zero baseline time but {input_time}s input time")
                continue

            # Calculate percentage change: (input - baseline) / baseline
            change_ratio = (input_time - baseline_time) / baseline_time
            change_percent = change_ratio * 100

            if change_ratio > self.regression_threshold:
                # Performance regression (slower)
                regressions.append((input_test, baseline_test, change_percent, input_time - baseline_time))
            elif change_ratio < -self.regression_threshold:
                # Performance improvement (faster)
                improvements.append((input_test, baseline_test, change_percent, baseline_time - input_time))
            else:
                # Within threshold
                stable_tests.append((input_test, baseline_test, change_percent))

        # Report results
        self._print_regressions(regressions)
        self._print_improvements(improvements)
        self._print_summary(len(common_tests), len(regressions), len(improvements), len(stable_tests))

    def _print_regressions(self, regressions):
        """Print performance regressions in red"""
        if regressions:
            print(f"{Colors.RED}PERFORMANCE REGRESSIONS:{Colors.RESET}")
            for input_test, baseline_test, change_percent, time_diff in regressions:
                props_str = ", ".join(f"{k}={v}" for k, v in input_test.properties.items())
                display_name = input_test.get_display_name()
                if props_str:
                    print(f"  {Colors.BLUE}Test:{Colors.RESET} {display_name} ({props_str})")
                else:
                    print(f"  {Colors.BLUE}Test:{Colors.RESET} {display_name}")
                print(
                    f"     {Colors.YELLOW}Baseline:{Colors.RESET} {baseline_test.get_class_path()} -> {Colors.BOLD}{baseline_test.time:.3f}s{Colors.RESET}")
                print(
                    f"     {Colors.YELLOW}Input:{Colors.RESET}    {input_test.get_class_path()} -> {Colors.BOLD}{input_test.time:.3f}s{Colors.RESET}")
                print(
                    f"     Regression: {Colors.RED}{change_percent:+.1f}%{Colors.RESET} ({Colors.RED}+{time_diff:.3f}s{Colors.RESET})")
            print()

    def _print_improvements(self, improvements):
        """Print performance improvements in green"""
        if improvements:
            print(f"{Colors.GREEN}PERFORMANCE IMPROVEMENTS:{Colors.RESET}")
            for input_test, baseline_test, change_percent, time_saved in improvements:
                props_str = ", ".join(f"{k}={v}" for k, v in input_test.properties.items())
                display_name = input_test.get_display_name()
                if props_str:
                    print(f"  {Colors.BLUE}Test:{Colors.RESET} {display_name} ({props_str})")
                else:
                    print(f"  {Colors.BLUE}Test:{Colors.RESET} {display_name}")
                print(
                    f"     {Colors.YELLOW}Baseline:{Colors.RESET} {baseline_test.get_class_path()} -> {Colors.BOLD}{baseline_test.time:.3f}s{Colors.RESET}")
                print(
                    f"     {Colors.YELLOW}Input:{Colors.RESET}    {input_test.get_class_path()} -> {Colors.BOLD}{input_test.time:.3f}s{Colors.RESET}")
                print(
                    f"     Improvement: {Colors.GREEN}{change_percent:.1f}%{Colors.RESET} ({Colors.GREEN}-{time_saved:.3f}s{Colors.RESET})")
            print()

    def _print_summary(self, total_tests, regressions, improvements, stable):
        """Print summary of analysis"""
        print("SUMMARY:")
        print(f"  Total tests analyzed: {total_tests}")
        print(f"  Regressions (>{self.regression_threshold * 100:.0f}% slower): {regressions}")
        print(f"  Improvements (>{self.regression_threshold * 100:.0f}% faster): {improvements}")
        print(f"  Stable (within ±{self.regression_threshold * 100:.0f}%): {stable}")

        if regressions > 0:
            print(f"\n{Colors.RED}Performance regressions detected!{Colors.RESET}")
            sys.exit(1)
        elif improvements > 0:
            print(f"\n{Colors.GREEN}Performance improvements detected!{Colors.RESET}")
        else:
            print("\nNo significant performance changes detected.")


def main():
    if len(sys.argv) != 3:
        print("Usage: python test_analyzer.py <input.xml> <baseline.xml>")
        print("  input.xml    - Test results to analyze")
        print("  baseline.xml - Baseline test results for comparison")
        sys.exit(1)

    input_file = sys.argv[1]
    baseline_file = sys.argv[2]

    print(f"Input file: {input_file}")
    print(f"Baseline file: {baseline_file}")
    print(f"Regression threshold: 5%\n")

    analyzer = TestRegessionAnalyzer(regression_threshold=0.05)

    # Parse both XML files
    input_tests = analyzer.parse_xml_file(input_file)
    baseline_tests = analyzer.parse_xml_file(baseline_file)

    print(f"Loaded {len(input_tests)} tests from input file")
    print(f"Loaded {len(baseline_tests)} tests from baseline file\n")

    # Compare tests and report results
    analyzer.compare_tests(input_tests, baseline_tests)


if __name__ == "__main__":
    main()