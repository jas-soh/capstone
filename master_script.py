import pandas as pd
import numpy as np
import scipy
from scipy.fft import rfft, rfftfreq, irfft
from scipy.signal import butter, filtfilt, periodogram, lfilter, freqz
import scipy.fft
import scipy.fftpack
import matplotlib
matplotlib.use("TkAgg")
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
def fourier(data, fs):


    fig = plt.figure(figsize=(6,12))

    # centering the data around 0
    data_centered = data - np.mean(data)
    # yf = scipy.fft.fft(data_centered)
    N = len(data)

    yf = rfft(data_centered)
    xf = rfftfreq(N, 1 / fs)

    # plotting centered data
    plt.subplot(3,1,1)
    plt.stem(xf,np.abs(yf))
    plt.title('vibration_data')

    plt.subplot(3,1,2)
    t = np.linspace(0,len(data)*100,num=len(data))
    plt.plot(t,data_centered)

    plt.show()

    return np.abs(yf),xf

def fundamental_freqs(xf, yf):
    threshold = 400
    yf[np.abs(yf)<threshold] = 0

    # print(np.where(yf > 0))
    points = np.transpose(np.stack((xf,yf)))
    # print(points[:,1])
    fundamental = points[points[:,1] > 0]

    return yf

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
    # plt.semilogy(f, Pxx_den)
    # plt.xlabel('frequency [Hz]')
    # plt.ylabel('PSD [V**2/Hz]')
    # plt.show()

    return f, Pxx_den
    

def psd_analysis(f, Pxx_den):
    # threshold = 10
    # Pxx_den[np.abs(Pxx_den)<threshold] = 0
    # return Pxx_den

    # filter array
    # print(f.shape,Pxx_den.shape)
    arr = np.vstack((f,Pxx_den))
    # print(arr)
    filter = arr[1] > 2000
    filtered_arr = arr[:,filter]

    sorted_arr = np.sort(filtered_arr)
    # print(sorted_arr)

    return sorted_arr
    

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
    # x = np.linspace(0,len(data)*fs,num=len(data))
    # plt.plot(x,data,color='blue')
    # plt.plot(x,y,color='red')
    # plt.show()
    return y


def fourier_2(data):
    fs = 100
    yf = scipy.fft.fft(data)
    filtered_y = butter_lowpass_filter(data, fs)
    filtered_yf = scipy.fft.fft(filtered_y)
    x = scipy.fftpack.fftfreq(yf.size, 1 / 100)
    filtered_x = scipy.fftpack.fftfreq(filtered_yf.size, 1 / 100)

    fig = plt.figure(figsize=(6,12))

    x_nom = filtered_x[:filtered_x.size//2]
    y_nom = abs(filtered_yf)[:filtered_yf.size//2]
    return x_nom, y_nom

def main():
    # ----- load accelerometer data file -----
    accel_filename = "data/ACCEL.txt"
    accel_filename = ("data/accelerometer.csv")
    accel_filename = ("data/vibrations.csv")

    accel_filename_nom = ("data/alt_test15_nom.txt")
    accel_filename_fail = ("data/alt_test9_fail.txt")
    accel_filename_fail2 = ("data/alt_test20_fail2.txt")

    accel_df_nom = csv_to_df(accel_filename_nom)
    accel_df_fail = csv_to_df(accel_filename_fail)
    accel_df_fail2 = csv_to_df(accel_filename_fail2)
    
    # ----- load strain data file -----
    # strain_filename = "STRAIN.txt"
    # strain_df = csv_to_df(strain_filename)

    # ----- define material constants -----
    material_const = 0.002 # todo
    fs = 100

    # ----- filter data using lowpass filter -----
    filteredz_nom = butter_lowpass_filter(np.array(accel_df_nom['accel1_z']),fs)
    filteredz_fail = butter_lowpass_filter(np.array(accel_df_fail['accel1_z']),fs)
    filteredz_fail2 = butter_lowpass_filter(np.array(accel_df_fail2['accel1_z']),fs)
    # filtered1_y = butter_lowpass_filter(np.array(accel_df['accel1_y']), fs)
    # filtered1_z= butter_lowpass_filter(np.array(accel_df['accel1_z']), 100)


    # ----- fourier transform -----
    yf_nom, xf_nom = fourier(filteredz_nom, 100)
    yf_fail, xf_fail = fourier(filteredz_fail, 100)
    yf_fail2, xf_fail2 = fourier(filteredz_fail2, 100)


    # ----- identify fundamental frequencies from fft -----
    # yf = fundamental_freqs(xf,yf)

    # ----- periodogram analysis and comparison -----
    f_nom, Pxx_den_nom = psd(filteredz_nom,fs)
    f_fail, Pxx_den_fail = psd(filteredz_fail,fs)
    f_fail2, Pxx_den_fail2 = psd(filteredz_fail2,fs)

    # plt.scatter(f_nom, Pxx_den_nom,color="red")
    # plt.scatter(f_fail, Pxx_den_fail,color="blue")
    # plt.scatter(f_fail2, Pxx_den_fail2,color="green")
    # plt.xlabel('frequency [Hz]')
    # plt.ylabel('PSD [V**2/Hz]')
    # plt.show()


    norm_arr = psd_analysis(f_nom, Pxx_den_nom)
    fail_arr = psd_analysis(f_fail, Pxx_den_fail)
    fail2_arr = psd_analysis(f_fail2, Pxx_den_fail2)

    plt.scatter(norm_arr[0][:],norm_arr[1][:],color="red")
    plt.scatter(fail_arr[0][:],fail_arr[1][:],color="blue")
    plt.scatter(fail2_arr[0][:],fail2_arr[1][:],color="green")

    #plt.scatter(norm_arr[0][-20:-3],norm_arr[1][-20:-3],color="red")
    #plt.scatter(fail_arr[0][-20:-3],fail_arr[1][-20:-3],color="blue")
    #plt.scatter(fail2_arr[0][-20:-3],fail2_arr[1][-20:-3],color="green")
    plt.xlim(0,50)
    plt.show()


    # # Do a Fourier transform on the signal
    
    # tx  = np.fft.fft(a)
    # print(tx.shape)
    data_z_nom = accel_df_nom['accel1_z'].values
    x_nom, y_nom = fourier_2(data_z_nom)
    x_nom = x_nom[4:]
    y_nom = y_nom[4:]
    data_z_fail = accel_df_fail['accel1_z'].values
    x_fail, y_fail = fourier_2(data_z_fail)
    x_fail = x_fail[4:]
    y_fail = y_fail[4:]
    data_z_fail2 = accel_df_fail2['accel1_z'].values
    x_fail2, y_fail2 = fourier_2(data_z_fail2)
    x_fail2 = x_fail2[4:]
    y_fail2 = y_fail2[4:]

    plt.plot(x_nom, y_nom, color="red")
    plt.plot(x_fail, y_fail, color="blue")
    plt.plot(x_fail2, y_fail2, color="green")
    plt.title('FFT Vibe z axis')
    plt.show()


main()
