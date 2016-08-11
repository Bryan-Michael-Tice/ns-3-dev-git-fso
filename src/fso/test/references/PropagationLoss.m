wavelength = 1060*10^(-9);
d = 707000;%36000000;%distance of link (m)
r = 0;%radial distance from optical axis
rx_aperture_diameter = 0.5;
curvature = d;
w0 = 0.06;
HGS = 0;

scintidx = CalcScintillationIdx(30*(pi/180), wavelength, d, HGS)

diffractiveBeamRadius = CalcDiffractiveBeamRadius(wavelength, d, w0, curvature)
mean_irradiance = CalcMeanIrradiance( w0, diffractiveBeamRadius, r )
free_space_loss = CalcFreeSpaceLoss(d, wavelength)
pd = makedist('Lognormal','mu',-0.5*scintidx,'sigma',sqrt(scintidx));
 
x = random(pd,100000,1);
x = log(x);
[f,xi] = ksdensity(x);
plot(xi,f); %plot PDF

%Instantaneous Received power is 0.125*pi*D^2 * I, where D is the diamater
%of the lens (11.4.1 of book)
Prx = 0.125*pi*(rx_aperture_diameter^2)*mean_irradiance
