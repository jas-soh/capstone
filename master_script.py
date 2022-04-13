import pandas as pd
import scipy
import scipy.fft
import scipy.fftpack
import matplotlib.pyplot as plt


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

def strain_analysis(material_const,strain_gauge_readings):
    return

def rainflow_analysis():
    return

def vibration_analysis(accel_readings):
    # to get the sampling rate, one time
    yf = scipy.fft.fft(accel_readings['accel1_x'].values)
    x = scipy.fftpack.fftfreq(yf.size, 1 / 100)

    fig = plt.figure(figsize=(6,12))

    for i in accel_readings.columns:
        yf = scipy.fft.fft(accel_readings[i].values)
        #plt.subplot(3,1,1)
        plt.plot(x[:x.size//2], abs(yf)[:yf.size//2])
        plt.title('vibration_data x')

        # yf = scipy.fft.fft(accel_readings[i+2].values)
        # plt.subplot(3,1,2)
        # plt.plot(x[:x.size//2], abs(yf)[:yf.size//2])
        # plt.title('vibration_data y')

        # yf = scipy.fft.fft(accel_readings[i+3].values)
        # plt.subplot(3,1,3)
        # plt.plot(x[:x.size//2], abs(yf)[:yf.size//2])
        # plt.title('vibration_data z')

        plt.show()
    return

def main():
    filename = "LOGDATA.TXT"
    material_const = 0.002 # todo

    data_df = csv_to_df(filename)

    strain_gauge_readings, accel_readings = split_data(data_df)
    strain_analysis(material_const, strain_gauge_readings)
    vibration_analysis(accel_readings)
    

main()