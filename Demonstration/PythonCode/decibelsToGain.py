import numpy as np

def decibels_to_gain(decibels, minus_infinity_db):
    '''
        Converts decibels to gain using the formula: gain = 10^(decibels * 0.05).

        Parameters -
        decibels (float): The input value in decibels.
        minus_infinity_db (float): The threshold below which the gain is set to 0.
    '''

    # Vectorised
    if type(decibels) is np.ndarray:
        gain = np.zeros(decibels.shape)
        mask = decibels > minus_infinity_db
        gain[mask] = 10**(decibels[mask]*0.05)

    else:
        if decibels > minus_infinity_db:
            gain = 10.0 ** (decibels * 0.05)
        else:
            gain = 0

    return gain
