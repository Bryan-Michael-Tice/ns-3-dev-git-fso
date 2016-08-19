wavelength = 1060*10^(-9);  %wavelength of signal in meters
HSAT = 707000;              %height of satellite in meters
HGS = 0;                    %height of ground station in meters
zenith = 30.0*(pi/180.0);   %zenith angle in radians

scintidx = CalcScintillationIdx(zenith, wavelength, HSAT, HGS);
pd = makedist('Lognormal','mu',-0.5*scintidx,'sigma',sqrt(scintidx));
 
x = random(pd,100000,1);
[f,xi] = ksdensity(x);

%plot the probability density function
figure
%plot(xi,f); %linear scale
loglog(xi,f); %log scale
xlim([0,1e1])
ylim([1e-3,1e2]);
title('PDF of Normalized RX Irradiance')
