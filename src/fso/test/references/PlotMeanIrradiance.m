wavelength = 1060*10^(-9);%847*10^(-9); %wavelength in meters
d = 707000; %distance of link in meters
rxPhaseFrontRadius = d; %phase front radius of curvature at the receiver in meters (approximately the distance)
txBeamRadius = 0.06;% beam radius in meters
r_vec = [0.0:0.001:0.06];
n = 0;
for r = 0.0:0.001:0.06
    n = n + 1;
    WLT = CalcDiffractiveBeamRadius( wavelength, d, txBeamRadius, rxPhaseFrontRadius );
    meanIrradiance(n) = CalcMeanIrradiance( txBeamRadius, WLT, r );
end

plot(meanIrradiance)
title('Scintillation Index vs Elevation Angle')
ylabel('Scintillation Index')
xlabel('Elevation (degrees)')
