import pandas as pd
import scipy
import scipy.fft
import scipy.fftpack
import numpy as np
from scipy.signal import butter, lfilter, freqz
import matplotlib
matplotlib.use("TkAgg")
import matplotlib.pyplot as plt
# Low pass filter source: https://www.delftstack.com/howto/python/low-pass-filter-python/


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

def vibration_analysis(data):
    # to get the sampling rate, one time
   # data = pd.read_csv('vibration_data.csv')
    yf = scipy.fft.fft(data['accel1_x'].values)
    filtered_y = butter_lowpass_filter(data['accel1_x'].values, 50, 100.1)
    filtered_yf = scipy.fft.fft(filtered_y)
    x = scipy.fftpack.fftfreq(yf.size, 1 / 100)
    filtered_x = scipy.fftpack.fftfreq(filtered_yf.size, 1 / 100)

    fig = plt.figure(figsize=(6,12))

    plt.subplot(3,1,1)
    #plt.plot(x[:x.size//2], abs(yf)[:yf.size//2])
    plt.plot(filtered_x[:filtered_x.size//2], abs(filtered_yf)[:filtered_yf.size//2])
    plt.title('vibration_data x')
    plt.show()
    
    return


def butter_lowpass(cutoff, fs, order=5):
    nyq = 0.5 * fs # Highest frequency we will read
    normal_cutoff = cutoff / nyq
    b, a = butter(order, normal_cutoff, btype='low', analog=False)
    return b, a

def butter_lowpass_filter(data, cutoff, fs, order=1):
    b, a = butter_lowpass(cutoff, fs, order=order)
    y = lfilter(b, a, data)

    # Plotting the frequency response.
    w, h = freqz(b, a, worN=8000)
    plt.subplot(2, 1, 1)
    plt.plot(0.5*fs*w/np.pi, np.abs(h), 'b')
    plt.plot(cutoff, 0.5*np.sqrt(2), 'ko')
    plt.axvline(cutoff, color='k')
    plt.xlim(0, 0.5*fs)
    plt.title("Lowpass Filter Frequency Response")
    plt.xlabel('Frequency [Hz]')
    plt.grid()
    return y


def main():
    #filename = "/Users/arveanlabib/Documents/capstone/vibration/vibration_data.csv"
    material_const = 0.002 # todo

    #data_df = csv_to_df(filename)
    data = pd.read_csv("/Users/arveanlabib/Documents/capstone/data/vibrations.csv")
    #data = csv_to_df("/Users/arveanlabib/Documents/capstone/data/vibrations.csv")

    #strain_gauge_readings, accel_readings = split_data(data_df)
   # strain_analysis(material_const, strain_gauge_readings)
    print(data['accel1_x'].values)
    vibration_analysis(data)
main()
