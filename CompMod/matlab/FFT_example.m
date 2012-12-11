
clear all;

% We want to create 1 second of data. The sampling rate
% should be similar to GW detectors, i.e. about 10kHz.
% We choose a number which is 2^n as it makes the FFTs
% faster.

T=2; % 1 second
fs=16384; % sampling rate 

N=T*fs; % number of points

% time vector
t=linspace(0,T,N);

% creating 4 waves: sine, square, sawtooth, triangle,
% all a f=100Hz;

f0=100;
w=2*pi*f0;

d0=t*0.0+1; % creating DC only data
d1=sin(w*t); % sine wave
d2=square(w*t); % square wave
d3=sawtooth(w*t); % sawtooth wave
d4=sawtooth(w*t,0.5); %triangular wave

figure
subplot(5,1,1)
% plot all to check
plot(t,d1,t,d2,t,d3,t,d4);
xlim([0 .04]);

% Preparing Fourier transformation

wsize=8*2048; % window size
overlap=wsize/2;  % 50% overlap between widows

% create window vector
w  = window(@blackmanharris,wsize);

% compute Fourier transforms

[Pd0,f]=pwelch(d0,w,overlap,N,fs);
[Pd1,f]=pwelch(d1,w,overlap,N,fs);
[Pd2,f]=pwelch(d2,w,overlap,N,fs);
[Pd3,f]=pwelch(d3,w,overlap,N,fs);
[Pd4,f]=pwelch(d4,w,overlap,N,fs);

% and plot...

mxlim=[50 1e3];
mylim=[1e-6 1];
subplot(5,1,2)
loglog(f,Pd1);
xlim(mxlim);
ylim(mylim);
subplot(5,1,3)
loglog(f,Pd2)
xlim(mxlim);
ylim(mylim);
subplot(5,1,4)
loglog(f,Pd3)
xlim(mxlim);
ylim(mylim);
subplot(5,1,5)
loglog(f,Pd4)
xlim(mxlim);
ylim(mylim);


