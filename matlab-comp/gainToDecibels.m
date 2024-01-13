function decibels = gainToDecibels (gain, minusInfinityDb)
    if (gain > minusInfinityDb)
        decibels = max(minusInfinityDb, log10(gain) * 20.0);
    else
        decibels = minusInfinityDb;
    end
end