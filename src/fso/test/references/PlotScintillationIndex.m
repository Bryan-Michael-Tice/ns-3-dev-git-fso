wavelength = 1060*10^(-9);%847*10^(-9); %wavelength in meters
d = 707000; %distance of link in meters
HGS = 0;%height of ground station in meters

scintIndex = zeros(45);

for n = 1:45
    scintIndex(n) = CalcScintillationIdx(n*(pi/180), wavelength, d, HGS);
end

plot(scintIndex(1:45))
title('Scintillation Index vs Elevation Angle (1060 nm)')
ylabel('Scintillation Index')
xlabel('Elevation (degrees)')
