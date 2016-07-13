function [ numFades ] = CalcNumFades( scintillationIndex, Ft)
%Number of fades: Section 11.3.2 of Laser Beam Propagation through Random Media (valid under weak fluctuations, for lognormal
%distribution)

%CALCNUMFADES Outputs the number of expected fades given a threshold Ft
%(dB)

b2 = (1/(2*pi));%To calculate b2 (autocorrelation) we need to PSD of the shot noise

v0 = sqrt(b2)/(pi*sqrt(sctillationIndex));% Quasi-frequency (Hz)
numFades = v0*exp(-((0.5*scintillationIndex - 0.23*Ft)^2)/(2*scintillationIndex));

end

