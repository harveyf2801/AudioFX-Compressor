import matplotlib.pyplot as plt
import numpy as np

def plot_gain_computer(g):
    '''
        Plot the output of the gain computer of a compressor.

        Parameters:
        g (array): representing the gain values over time
    '''
    
    # Create a new figure
    plt.figure()

    # Plot the gain values
    plt.plot(g, linewidth=2)

    # Adding a title and labels, and enabling a grid
    plt.title('Compressor Gain Computer Output')
    plt.xlabel('Time (s)')
    plt.ylabel('Gain Reduction (dB)')
    plt.ylim([-35, 0])
    plt.grid(True)

    # Add legend
    plt.legend(['Gain Computer Output'], loc='lower right')

    # Show the plot
    plt.show()
