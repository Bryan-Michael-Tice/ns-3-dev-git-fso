wavelength = 1550*10^(-9);
d = 35000;%distance of link
r = 0;%radial distance from optical axis
rx_aperture_diameter = 0.2;

k = 1.381*10^-23;%boltzmann's constant
T = 290; %temperature in kelvin
thermalNoise = k*T; %W/Hz

scintidx = CalcScintillationIdx(10*(pi/180), wavelength, d,0)

diffractiveBeamRadius = CalcDiffractiveBeamRadius(wavelength, d, 0.120, 10^8)
mean_irradiance = CalcMeanIrradiance( 0.120, diffractiveBeamRadius, r )

pd = makedist('Lognormal','mu',-0.5*scintidx,'sigma',sqrt(scintidx))

x = random(pd,10000,1)
hist(x,100)
%logx = log(x)
%hist(logx,50)

%Instantaneous Received power is 0.125*pi*D^2 * I, where D is the diamater
%of the lens (11.4.1 of book)
Prx = 0.125*pi*rx_aperture_diameter*x*mean_irradiance
