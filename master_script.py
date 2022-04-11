import pandas as pd


def csv_to_df(filename):
    """
    Reads csv file into a pandas dataframe
    Input: name of csv file with data
    Output: dataframe with data from file
    """

    data = pd.read_csv(filename)
    return data

def split_data(data_df):
    """
    Separates the dataframe into individual dataframes of data from each sensor
    Input: dataframe of all the data from sensor readings
    Output: two dataframes - one of strain gauge readings, one of accelerometer readings
    """
    strain_gauge_readings = data_df.loc[:,'time':'valRaw4']
    accel_readings = data_df.loc[:,'accel1_x':'accel4_z']

    print(strain_gauge_readings)
    print(accel_readings)

    return strain_gauge_readings, accel_readings

def strain_analysis():
    return

def rainflow_analysis():
    return

def vibration_analysis():
    return

def main():
    filename = "LOGDATA.TXT"
    data_df = csv_to_df(filename)

    strain_gauge_readings, accel_readings = split_data(data_df)
    

main()