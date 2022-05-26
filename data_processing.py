import pandas as pd
import numpy as np
import math

def round_down(data):
    rounded = [math.floor(i/100)*100 for i in data]
    print(rounded)
    return rounded

def resample_100hz(df):
    """
    Inputs: df with time column and data column
    """
    df['time'] = df['time'].floordiv(10).multiply(10)
    df = df.groupby('time', as_index=False, sort=False).mean().loc[:,"time":"accel2_z"]
    
    return df

def get_strain(df, col, V_offset):
    # system values
    R1 = 820 # ohms
    R2 = 820 # ohms
    R3 = 350 # ohms
    R_sg = 350 # ohms
    K = 1.93 # gauge factor
    V_in = 3.3 # V
    gain = 495
    V_offset = V_offset * 5 / 1024 #0.5 # V
    E = 68.9

    print("v offset:",V_offset)

    ADC_val = df[col]
    term1 = -1 * R2 / ((((ADC_val) * (5 / 1024) - V_offset) / (gain * V_in)) - (R1 / (R1 + R3)))
    term2 = R2 + R_sg
    term3 = R_sg * K
    strain_measured = (term1 - term2) / term3
    
    df['raw_' + col] = strain_measured
    df['stress_' + col] = df['raw_' + col] * E
    return df

def csv_to_df(filename):
    """
    Reads csv file into a pandas dataframe
    Input: name of csv file with data
    Output: dataframe with data from file
    """

    data = pd.read_csv(filename)
    return data

def df_to_csv(df, filename):
    df.to_csv('resampled_data/' + filename, index=False)

def main():
    # filename = "data/crash_strain.txt"
    # data = pd.read_csv(filename)
    # times = data.loc[:,'time']
    # new_times = round_down(times)


    # filename = "data/val2_accel_casual.txt"
    # data = pd.read_csv(filename)
    # df = resample_100hz(data)
    # # print(df)
    # df.to_csv("data/alt_val2_accel_casual.txt", index=False)

    # ----- load strain data file -----
    strain_filename = "data/strain_test4_agg.txt"
    strain_raw_df = csv_to_df(strain_filename)
    strain_df = get_strain(strain_raw_df,'strain2',strain_raw_df.iloc[0,2])
    strain_df = get_strain(strain_df,'strain3',strain_raw_df.iloc[0,3])

    df_to_csv(strain_df, 'strain_test4_agg.csv')

main()