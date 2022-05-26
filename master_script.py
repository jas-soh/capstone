import pandas as pd
import numpy as np
from scipy.fft import rfft, rfftfreq, irfft
from scipy.signal import butter, filtfilt, periodogram, welch, lombscargle
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


    # fig = plt.figure(figsize=(6,12))

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
    t = np.linspace(0,len(data)*10,num=len(data))
    plt.plot(t,data_centered)
    plt.xlabel("time (ms)")

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

def lomb_scargle(x, y):
    # Define the array of frequencies for which to compute the periodogram:
    w = np.linspace(.1, 300, 10000)
    pgram = lombscargle(x, y, w, normalize=True)

    plt.plot(w, pgram)
    # plt.xlabel('Angular frequency [rad/s]')
    # plt.ylabel('Normalized amplitude')
    # plt.show()

    return w, pgram

def welch_psd(x,fs):
    f, Pxx_spec = welch(x, fs)
    
    return f, Pxx_spec

def psd(x,fs):
    f, Pxx_den = periodogram(x, fs)

    return f, Pxx_den
    
def psd_analysis(f, Pxx_den):
   
    arr = np.stack((f,Pxx_den), axis=-1)
  
    sorted_arr = arr[arr[:,1].argsort()]
    # print(sorted_arr)

    return sorted_arr[-10:,0],sorted_arr[-10:,1]

def top10_psd(freq, Pxx_den):
    # freq = np.transpose(freq)
    # Pxx_den = np.transpose(Pxx_den)
    # arr = np.stack((freq, Pxx_den))

    arr = np.stack((freq,Pxx_den), axis=-1)
    arr = arr[np.argsort(arr[:, 1])]
    print(arr)
    return arr[-10:,0],arr[-10:,1]

def butter_lowpass_filter(data, fs):
    '''
    inputs: 
        data - array of data that will be filtered
        fs - frequency that the data was sampled at in Hz
    '''
    # filtering coefficients
    Wn = 30
    order = 3

    b, a = butter(50, Wn, 'low', fs = fs, analog=False)
    y = filtfilt(b, a, data)
    return y

def comparison(nominal, data):
    """
    Compares the nominal psd data with the data to be analyzed
    Inputs: nominal data array and current data array
        both have shape of length x 2 (column 1 is frequency, column 2 is energy)
    """

    # comarison method: Euclidean norm for
    i = -1
    norm = np.linalg.norm(nominal[-1] - data[-1])
    print(norm)


def vibration_analysis(accel_filename, fs, col_name, color, mark="o",label=None): # remove accel_num, color    

    accel_df = csv_to_df(accel_filename)

    # ----- filter data using lowpass filter -----
    filtered_data = butter_lowpass_filter(np.array(accel_df[col_name] / 255 ),fs)
    sample_time = np.array(accel_df["time"])

    filtered_data = filtered_data - np.mean(filtered_data)

    # ----- fourier transform -----
    # yf, xf = fourier(filtered_data, fs)

    # ----- periodogram analysis and comparison -----
    # freq, Pxx_den = psd(filtered_data,fs)

    freq, Pxx_den = welch_psd(filtered_data,fs)

    # w, pgram = lomb_scargle(sample_time, filtered_data)

    x,y = psd_analysis(freq, Pxx_den)

    # freq10, Pxx_den10 = top10_psd(freq,Pxx_den)

    plt.scatter(x,y,color=color,marker=mark, label=label)
    # plt.scatter(w,pgram)

    # plt.xlim(30,60)
    # plt.xlabel('frequency [Hz]')
    # plt.ylabel('PSD (acceleration) [g**2/Hz]')
    # plt.show()

    # comparison(norm_arr, fail2_arr)

def get_strain(df, col):
    # system values
    R1 = 820 # ohms
    R2 = 820 # ohms
    R3 = 350 # ohms
    R_sg = 350 # ohms
    K = 1.93 # gauge factor
    V_in = 3.3 # V
    gain = 495
    V_offset = 105 * 5 / 1024 #df[col][0] * 5 / 1024 #0.5 # V
    E = 68.9

    ADC_val = df[col]
    term1 = R2 / ((((ADC_val) * (5 / 1024) - V_offset) / (gain * V_in)) + (R1 / (R1 + R3)))
    term2 = R2 + R_sg
    term3 = R_sg * K
    strain_measured = (term1 - term2) / term3
    
    df['raw_strain_' + col] = strain_measured
    df['stress_' + col] = df['raw_strain_' + col] * E
    return df

def main():

    # ----- define material constants -----
    material_const = 0.002 # todo
    fs = 100

    # ----- load accelerometer data file -----
    accel_filename = ("data/alt_test14_nom.txt")

    # ----- load strain data file -----
    strain_filename = "data/strain_test1_hover.txt"
    strain_raw_df = csv_to_df(strain_filename)
    strain_df = get_strain(strain_raw_df,'strain2')
    print(strain_df)


    # vibration_analysis("data/alt_test14_nom.txt", fs, "accel1_z", "black")
    # vibration_analysis("data/alt_test14_nom.txt", fs, "accel2_z", "black", mark="x",label="nominal")
    # vibration_analysis("data/alt_test15_nom.txt", fs, "accel1_z", "black")
    # vibration_analysis("data/alt_test15_nom.txt", fs, "accel2_z", "black", mark="x")

    # vibration_analysis("data/alt_test8_fail.txt", fs, "accel1_z", "orange")
    # vibration_analysis("data/alt_test8_fail.txt", fs, "accel2_z", "orange", mark="x")
    # vibration_analysis("data/alt_test9_fail.txt", fs, "accel1_z", "orange")
    # vibration_analysis("data/alt_test9_fail.txt", fs, "accel2_z", "orange", mark="x",label="small cut ")
    # vibration_analysis("data/alt_test10_fail.txt", fs, "accel1_z", "orange")
    # vibration_analysis("data/alt_test10_fail.txt", fs, "accel2_z", "orange", mark="x")


    # vibration_analysis("data/alt_test20_fail2.txt", fs, "accel1_z", "red")
    # vibration_analysis("data/alt_test20_fail2.txt", fs, "accel2_z", "red", mark="x",label="big cut")
    # vibration_analysis("data/alt_test19_fail2.txt", fs, "accel1_z", "red")
    # vibration_analysis("data/alt_test19_fail2.txt", fs, "accel2_z", "red", mark="x")

    # ---------- in flight tests ----------
    # vibration_analysis("data/alt_val1_accel_hover.txt", fs, "accel1_z", "green")
    # vibration_analysis("data/alt_val2_accel_casual.txt", fs, "accel1_z", "blue")

    # plt.xlabel('frequency [Hz]')
    # plt.ylabel('Z axis PSD (acceleration) [g**2/Hz]')
    # plt.legend()
    # plt.title("Z-Axis Power Spectral Density")
    # plt.ylim(0,0.8)
    # plt.xlim(10,35)
    # plt.show()



main()
