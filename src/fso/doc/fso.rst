Fso Module
----------

.. include:: replace.txt
.. highlight:: cpp

.. heading hierarchy:
   ------------- Chapter
   ************* Section (#.#)
   ============= Subsection (#.#.#)
   ############# Paragraph (no number)

The Fso module looks to provide a model for free-space optical communication links. It is currently under development as part of the SOCIS 2016 Optical Satellite Systems (OSS) project.

Model Description
*****************

The Fso module provides:

* a PropagationLossModel class for optical signals traveling through the atmosphere

* an Antenna model for a laser

* a Phy and Channel class to implement a free-space optical link


The source code for the Fso module is located at ``src/fso``.

Design
======

Propagation Loss Model
######################

The propagation loss is modeled in the FsoPropagationLossModel class. This class models the losses incurred due to an optical signal being transmitted at high altitude ( > 20 km) to a receiver at a lower altitude (i.e. a geosynchronous satellite to a low earth orbit satellite or an optical ground station on Earth). This model would specifically suit the needs of a 'downlink' for satellites.

The model provides the irradiance (Watts/meter) at the receiver, which is described by a log-normal distribution [LaserPropagationBook]_. The distribution of the irradiance at the receiver, :math:'p_{I}(I) ,is a function of the scintillation index :math:'\sigma^{2}_{I}' and the mean irradiance :math:'\langle I(\textbf{r}, L) \rangle':

.. math::
   p_{I}(I) = \frac{1}{I\sigma_{I}(\textbf{r}, L)\sqrt(2\pi)}exp\Bigg(-\frac{\bigg[ln\big(\frac{I}{\langle I(\textbf{r}, L) \rangle}\big) + \frac{1}{2}\sigma^{2}_{I}(\textbf{r}, L)\bigg]^{2}}{2\sigma^{2}_{I}(\textbf{r}, L)}\Bigg)

where :math:'\textbf{r}' is the transverse observation point. The mean irradiance and can be expressed as follows:

.. math::
  \langle I(\textbf{r}, L) \rangle = \frac{W^{2}_{0}}{W^{2}_{LT}}exp\bigg(-\frac{2r^{2}}{W^{2}_{LT}}\bigg)

where :math:'W^{2}_{0}' is the beam radius at the transmitter, :math:'W^{2}_{LT}' is the effective spot size of the beam in the presence of atmospheric turbulence, and :math:'r' is the radial distance from the center of the beam. 

The scintillation index can be simplified in the downlink case, due to the beam effectively appearing as an unbounded plane wave once it enters the atmosphere. The scintillation index used for the downlink model is: 

.. math:: 
   \sigma_{I}^{2}(r, L) = 2.25k^{7/6}\sec^{11/6}(\zeta)\int^{H}_{H_{GS}}C^{2}_{n}(h)[h-h_{GS}]^{5/6}\,dh

where :math:'k = 2\pi/\lambda' and :math:'\lambda' is the wavelength of the optical beam, :math:'H' is the altitude of the transmitter, :math:'\zeta' is the elevation angle, :math:'H_{GS}' is the altitude of the ground station, and :math:'C_{n}^{2}(h)' is the index of refraction of the atmosphere as a function of altitude. The index of refraction can be described by a modified version of the Hufnagel-Valley model [SatOpticalFeederLinks2014]_:

.. math::
   C_{n}^{2}(h)  =& Ae^{-H_{GS}/700}e^{-(h-H_{GS})/100}\\ 
                    &+ 5.94x10^{-53}\frac{v}{27}^{2}h^{10}e^{-h/1000}\\
		    &+ 2.7x10^{-16}e^{-h/1500}
   :label: index-refraction

where :math:'A' is the refractive index structure parameter at ground level and :math:'v' is the root-mean-square wind speed.   

Channel Model
#############

The FsoChannel class acts as a container object, holding pointers to the the FsoPhy receivers and transmitters, the FsoPropagationLossModel, and the PropagationDelayModel.

Phy Model
#########

The FsoPhy class...

Laser/Optical Receiver Model
############################

The LaserModel class characterizes a laser by it's beam width and phase front radius of curvature at the transmitter aperture, output power, and it's orientation (not yet implemented).

The OpticalReceiverModel class characterizes the receiver by it's gain, aperture size, and orientation. This has not yet been implemented.  

Scope and Limitations
=====================

What can the model do?  What can it not do?  Please use this section to
describe the scope and limitations of the model.

The Fso module currently can provide a model of the atmospheric channel for optical signals. It is designed with the OSS project as the primary application. There is no consideration for interference between signals and it is assumed there is a single transmitter per channel which may service multiple receivers in a concentrated area (i.e. around a ground station).

References
==========

.. [LaserPropagationBook] L.C. Andrews and R.L. Philips, "Laser Satellite Communication Systems" in Laser Beam Propagation through Random Media, 2nd ed. Bellingham, Washington; SPIE, 2005, ch. 12 

.. [SatOpticalFeederLinks2014] S. Dimitrov et al. "Digital Modulation and Coding for Satellite Optical Feeder Links", ASMS/SPSC 2014

Usage
*****

This section is principally concerned with the usage of your model, using
the public API.  Focus first on most common usage patterns, then go
into more advanced topics.

Building New Module
===================

Include this subsection only if there are special build instructions or
platform limitations.

Helpers
=======

What helper API will users typically use?  Describe it here.

Attributes
==========

What classes hold attributes, and what are the key ones worth mentioning?

Output
======

What kind of data does the model generate?  What are the key trace
sources?   What kind of logging output can be enabled?

Advanced Usage
==============

Go into further details (such as using the API outside of the helpers)
in additional sections, as needed.

Examples
========

What examples using this new code are available?  Describe them here.

Troubleshooting
===============

Add any tips for avoiding pitfalls, etc.

Validation
**********

Describe how the model has been tested/validated.  What tests run in the
test suite?  How much API and code is covered by the tests?  Again, 
references to outside published work may help here.
