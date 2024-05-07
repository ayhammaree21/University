import csv


def truth_table_to_decimal(truth_table):
    decimal_values = []
    for row in truth_table:
        decimal_value = 0
        for i, value in enumerate(row):
            if value == '1':
                decimal_value += 2 ** (len(row) - i - 1)
        decimal_values.append(decimal_value)
    print("Len: " + str(len(decimal_values)))
    return decimal_values


def read_truth_table_from_csv(file_path):
    N1 = []
    N1x = []
    with open(file_path, 'r') as csv_file:
        csv_reader = csv.reader(csv_file)
        for row in csv_reader:
            if row[8] == '1':
                N1.append(row[0:8])

            if row[8] == 'x' or row[8] == 'X':
                N1x.append(row[0:8])
    print("N1:")
    print(truth_table_to_decimal(N1))
    print("N1 don't care:")
    print(truth_table_to_decimal(N1x))
    print("\n")


read_truth_table_from_csv('MRD.csv')
