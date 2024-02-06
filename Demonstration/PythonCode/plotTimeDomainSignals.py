import matplotlib.pyplot as plt
import numpy as np

def plot_time_domain_signals(original_signal, wet_signal, fs):
    """
    Plots one signal over the other in the time domain, demonstrating any time feature
    that may have changed from one signal to another.

    Parameters:
    - original_signal (array): The first signal to plot.
    - wet_signal (array): The second signal to plot on top of the original signal.
    - fs (float): The sampling frequency of the audio.
    """
    
    # Create a time vector for plotting
    t = np.arange(0, len(original_signal)) / fs

    # Create a figure
    plt.figure()

    # Plot the original signal in the background with reduced opacity
    plt.plot(t, original_signal, color=[0.6, 0.6, 0.6], linewidth=1.5, label='Original Signal')
    #plt.hold(True)

    # Plot the compressed signal in the foreground
    plt.plot(t, wet_signal, 'b', linewidth=1.5, label='Compressed Signal')

    # Adding a title and labels, and enabling the grid
    plt.title('Comparison of Original and Compressed Signals')
    plt.xlabel('Time (s)')
    plt.ylabel('Amplitude (magnitude)')
    plt.grid(True)

    # Add legend
    plt.legend(loc='best')

    # Show the plot
    plt.show()
