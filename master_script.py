import pandas as pd
import numpy as np
from scipy.fft import rfft, rfftfreq
from scipy.signal import butter, filtfilt, periodogram
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
def vibration_analysis(data, fs):


    fig = plt.figure(figsize=(6,12))

    # centering the data around 0
    data_centered = data - np.mean(data)
    # yf = scipy.fft.fft(data_centered)
    N = len(data)

    yf = rfft(data_centered)
    xf = rfftfreq(N, 1 / fs)

    # smooth_yf = threshold_noise_filter(yf,0)

    # plotting centered data
    plt.subplot(3,1,1)
    plt.plot(xf,np.abs(yf))
    plt.title('vibration_data x')

    plt.subplot(3,1,2)
    t = np.linspace(0,len(data)*100,num=len(data))
    plt.plot(t,data_centered)

    plt.show()

    return data_centered

def threshold_noise_filter(yf,percent):
    # avg_amplitude = np.sum(abs(yf)) / len(yf)
    # threshold = np.percentile(avg_amplitude, CI)
    max_amplitude = np.max(abs(yf))
    # threshold = np.percentile(max_amplitude, CI)
    threshold = max_amplitude * percent/100
    for i in range(len(yf)):
        if (abs(yf)[i] < threshold): yf[i] = 0
    return yf


def psd(x,fs):
    f, Pxx_den = periodogram(x, fs)
    plt.plot(f, Pxx_den)
    plt.xlabel('frequency [Hz]')
    plt.ylabel('PSD [V**2/Hz]')
    plt.show()

    return f, Pxx_den
    

def psd_analysis(f, Pxx_den):
    data = []
    for i in range(len(f)):
        data.append([f[i], Pxx_den[i]])

    # sortedArr = arr2D[arr2D[:,columnIndex].argsort()]
    # sorted_data = data[data[:,1].argsort()]
    print(data)

def butter_lowpass_filter(data, fs):
    '''
    inputs: 
        data - array of data that will be filtered
        fs - frequency that the data was sampled at in Hz
    '''
    # filtering coefficients
    Wn = 30
    order = 3

    b, a = butter(order, Wn, 'low', fs = fs, analog=False)
    y = filtfilt(b, a, data)

    # plot filtered data
    x = np.linspace(0,len(data)*fs,num=len(data))
    # plt.plot(x,data,color='blue')
    # plt.plot(x,y,color='red')
    # plt.show()
    return y


def main():
    #filename = "LOGDATA.CSV"
    accel_filename = "data/ACCEL.txt"
    # accel_filename = ("data/accelerometer.csv")
    accel_filename = ("data/vibrations.csv")
    accel_df = csv_to_df(accel_filename)
    # strain_filename = "STRAIN.txt"
    # strain_df = csv_to_df(strain_filename)

    material_const = 0.002 # todo
    fs = 100


    #strain_gauge_readings, accel_readings = split_data(data_df)
    #strain_analysis(material_const, strain_gauge_readings)

    filtered1_x = butter_lowpass_filter(np.array(accel_df['accel1_x']),fs)
    # filtered1_y = butter_lowpass_filter(np.array(accel_df['accel1_y']), fs)
    # filtered1_z= butter_lowpass_filter(np.array(accel_df['accel1_z']), 100)

    # vibration_analysis(filtered_x1, fs

    f, Pxx_den = psd(filtered1_x,fs)
    psd_analysis(f, Pxx_den)
    
    # vibration_analysis(filtered1_x, fs)

main()
