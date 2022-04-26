import pandas as pd
import numpy as np
import scipy
import scipy.fft
import scipy.fftpack
import scipy.signal
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

#def vibration_analysis(accel_readings):
def vibration_analysis(data):
    # to get the sampling rate, one time
    
    # data = pd.read_csv("LOGDATA.csv", sep=',')
    # yf = scipy.fft.fft(data['accel2_x'].values)
    # x = scipy.fftpack.fftfreq(yf.size, 1 / 100)

    yf = scipy.fft.fft(data)
    x = scipy.fftpack.fftfreq(yf.size, 1 / 100)

    fig = plt.figure(figsize=(6,12))
    data_centered = data - np.mean(data)
    yf = scipy.fft.fft(data_centered)
    plt.subplot(3,1,1)
    plt.plot(x[:x.size//2], abs(yf)[:yf.size//2])
    plt.title('vibration_data x')

    
    
    plt.subplot(3,1,2)
    #plt.plot(data['time'],data['accel2_z'])
    t = np.linspace(0,3390,num=339)
    plt.plot(t,data_centered)
    # plt.xlim(600,2000)

    plt.show()

    return data_centered

def butter_lowpass_filter(data, fs, order):
    nyq = fs / 2
    # Get the filter coefficients 
    # b, a = scipy.signal.butter(order, normal_cutoff, btype='low', analog=False)
    fc = 5  # Cut-off frequency of the filter
    w = fc / (fs / 2) # Normalize the frequency
    b, a = scipy.signal.butter(order, w, 'low',analog=False)
    y = scipy.signal.filtfilt(b, a, data['accel1_z'])
    x = np.linspace(0,33900,num=339)
    plt.plot(x,data['accel1_z'],color='blue')
    plt.plot(x,y,color='red')
    plt.show()
    return y

def filter(data):
    sig_fft = scipy.fft.fft(data)

    sig_fft_filtered = sig_fft.copy()
    freq = scipy.fft.fftfreq(len(data), d=1./100)

    # define the cut-off frequency
    cut_off = 30

    sig_fft_filtered[np.abs(freq) < cut_off] = 0

    filtered = scipy.fft.ifft(sig_fft_filtered)

    t = np.linspace(0,len(data)*0.1, num=339)

    plt.plot(t,filtered)
    plt.show()

    yf = scipy.fft.fft(filtered)
    x = scipy.fftpack.fftfreq(yf.size, 1 / 100)

    fig = plt.figure(figsize=(6,12))
    data_centered = filtered - np.mean(filtered)
    yf = scipy.fft.fft(data_centered)
    plt.subplot(3,1,1)
    plt.stem(x[:x.size//2], abs(yf)[:yf.size//2])
    plt.title('vibration_data x')
    plt.show()


def main():
    filename = "LOGDATA.CSV"
    material_const = 0.002 # todo

    data_df = csv_to_df(filename)

    #strain_gauge_readings, accel_readings = split_data(data_df)
    #strain_analysis(material_const, strain_gauge_readings)
    #vibration_analysis(accel_readings)
    # filtered_data = butter_lowpass_filter(data_df,100,2)
    filter(data_df['accel1_z'])
    # vibration_analysis(filtered_data)


main()