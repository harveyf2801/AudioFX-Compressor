from PythonCode.gainToDecibels import gain_to_decibels
from PythonCode.decibelsToGain import decibels_to_gain
import matplotlib.pyplot as plt
import numpy as np

def plot_attack_release_characteristics(attack_time, release_time, threshold):
    '''
        Plots the attack and release characteristics of a compression system.
        
        Parameters:
        - attack_time (float): Duration of the attack phase in seconds.
        - release_time (float): Duration of the release phase in seconds.
        - threshold (float): Threshold level for compression in decibels.
    '''
    # Define the parameters
    frequency = 4  # in Hertz
    duration = attack_time + release_time + 3  # in seconds
    sampling_rate = 1000  # number of samples per second

    threshold_g = decibels_to_gain(threshold, -120)
    amp1 = threshold_g - (threshold_g * 0.8)
    amp2 = threshold_g + (threshold_g * 0.8)

    # Generate time vector
    t = np.linspace(0, duration, int(duration * sampling_rate))

    # Generate sine wave with variable amplitude
    amplitude = amp1 * np.ones_like(t)  # Initialize amplitude to 1 for the entire duration

    # Change amplitude back to 1 for the second half-second
    amplitude[(t > 1) & (t <= 1 + attack_time + 1)] = amp2

    # Calculate the sine wave values
    y = amplitude * np.sin(2 * np.pi * frequency * t)

    # Calculate the timings for the attack and release stages
    attack_start = 1
    attack_end = attack_start + attack_time
    release_start = attack_end + 1
    release_end = release_start + release_time

    plt.figure()

    plt.subplot(3, 1, 1)

    plt.fill_between([attack_start, attack_end, attack_end, attack_start], [amp2, amp2, -amp2, -amp2], color="m",
                     alpha=0.3)
    plt.fill_between([attack_end, release_start, release_start, attack_end], [amp2, amp2, -amp2, -amp2], color="r",
                     alpha=0.3)
    plt.fill_between([release_start, release_end, release_end, release_start], [amp2, amp2, -amp2, -amp2], color="y",
                     alpha=0.3)

    plt.plot(t, y, 'b')

    threshold_line = np.ones_like(t) * threshold_g
    plt.plot(t, threshold_line, '--r', linewidth=1)
    plt.plot(t, -threshold_line, '--r', linewidth=1)

    plt.xlabel('Time (seconds)')
    plt.ylabel('Amplitude')
    plt.title('Input Signal')

    plt.legend(['Sine Wave', 'Threshold', 'Attack', 'Compression', 'Release'], loc='best')

    plt.grid(True)

    plt.subplot(3, 1, 2)

    peak = gain_to_decibels(amp2, -120)
    xsc = min(peak, threshold) if peak < threshold else threshold
    gain_reduction = decibels_to_gain(xsc - peak, -120)

    envelope = np.ones_like(t)
    envelope[(t > attack_start) & (t <= attack_end)] = np.linspace(1, gain_reduction,
                                                                  len(t[(t > attack_start) & (t <= attack_end)]))
    envelope[(t > attack_end) & (t <= release_start)] = gain_reduction
    envelope[(t > release_start) & (t <= release_end)] = np.linspace(gain_reduction, 1,
                                                                      len(t[(t > release_start) & (t <= release_end)]))

    plt.plot(t, envelope, 'b')

    plt.xlabel('Time (seconds)')
    plt.ylabel('Amplitude')
    plt.title('Envelope')

    plt.legend(['Envelope', 'Attack', 'Compression', 'Release'], loc='best')

    plt.subplot(3, 1, 3)

    plt.fill_between([attack_start, attack_end, attack_end, attack_start], [amp2, amp2, -amp2, -amp2], color="m",
                     alpha=0.3)
    plt.fill_between([attack_end, release_start, release_start, attack_end], [amp2, amp2, -amp2, -amp2], color="r",
                     alpha=0.3)
    plt.fill_between([release_start, release_end, release_end, release_start], [amp2, amp2, -amp2, -amp2], color="y",
                     alpha=0.3)

    plt.plot(t, y * envelope, 'b')

    plt.plot(t, threshold_line, '--r', linewidth=1)
    plt.plot(t, -threshold_line, '--r', linewidth=1)

    plt.xlabel('Time (seconds)')
    plt.ylabel('Amplitude')
    plt.title('Output Signal')

    plt.legend(['Sine Wave', 'Threshold', 'Attack', 'Compression', 'Release'], loc='best')

    plt.grid(True)

    plt.tight_layout()
    plt.show()