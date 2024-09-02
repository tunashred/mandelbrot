import xml.etree.ElementTree as ET
import csv
import os


REGRESSION_THRESHOLD = 0.10

def parse_xml_file(xml_file_path):
    try:
        tree = ET.parse(xml_file_path)
        root = tree.getroot()

        data = []
        for suite in root.findall(".//testsuite"):
            suite_name = suite.get("name")
            for test_case in suite.findall("testcase"):
                name = test_case.get("name")
                execution_time = float(test_case.get("time"))
                timestamp = test_case.get("timestamp")
                result = test_case.get("result") or "completed"
                regression = ""

                # Check for regression
                if os.path.isfile(csv_file_path):
                    with open(csv_file_path, 'r') as csvfile:
                        reader = csv.DictReader(csvfile)
                        for row in reader:
                            if row["Test Name"] == name:
                                previous_time = float(row["Execution Time"])
                                print(f"Comparing {name}: Previous Time = {previous_time}, New Time = {execution_time}")
                                if (execution_time - previous_time) / previous_time > REGRESSION_THRESHOLD:
                                    regression = "REGRESSION"
                                break

                data.append((suite_name, name, execution_time, timestamp, result, regression))

        return data
    except ET.ParseError as e:
        print(f"Error parsing XML: {e}")
        return []

def write_to_csv(data, csv_file_path, append=False):
    file_exists = os.path.isfile(csv_file_path)

    with open(csv_file_path, mode='a' if append else 'w', newline='') as file:
        writer = csv.writer(file)

        if not append or not file_exists:
            writer.writerow(["Test Suite", "Test Name", "Execution Time", "Timestamp", "Result", "Regression"])

        writer.writerows(data)

        writer.writerow([])

if __name__ == "__main__":
    import sys

    if len(sys.argv) != 2:
        print("Usage: python process_test_results.py <path_to_xml>")
        sys.exit(1)

    xml_file_path = sys.argv[1]
    csv_file_path = 'results.csv'

    print(f"Processing results from: {xml_file_path}")

    # Parse the XML file and get the data
    data = parse_xml_file(xml_file_path)

    if data:
        print("Processing completed.")
        if os.path.isfile(csv_file_path):
            choice = input("Do you want to append the data to the existing CSV file? (y/n): ").strip().lower()
            if choice == 'y':
                write_to_csv(data, csv_file_path, append=True)
            else:
                write_to_csv(data, csv_file_path, append=False)
        else:
            write_to_csv(data, csv_file_path, append=False)
    else:
        print("No data to process.")
