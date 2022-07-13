fs = 31250;                 % sampling rate
F = [1000 2000 4000 8000];  % band limits
A = [0 1 0];                % band type: 0='stop', 1='pass'
dev = [0.0001 10^(0.1/20)-1 0.0001]; % ripple/attenuation spec
[M,Wn,beta,typ] = kaiserord(F,A,dev,fs);  % window parameters
b = fir1(M,Wn,typ,kaiser(M+1,beta),'noscale'); % filter design
M = 33;    % window length = FIR filter length (Window Method)
% fs = 22050; % sampling rate assumed (Hz)
f1 = 3000;   % lower pass-band limit = transition bandwidth (Hz)
beta = 8;   % beta for Kaiser window for decent side-lobe rejection
fn = fs/2;             % Nyquist limit (Hz)
f2 = fn - f1;          % upper pass-band limit
N = 2^(nextpow2(8*M)); % large FFT for interpolated display
k1 = round(N*f1/fs);   % lower band edge in bins
if k1<2, k1=2; end;    % cannot have dc or fn response
kn = N/2 + 1;          % bin index at Nyquist limit (1-based)
k2 = kn-k1+1;          % high-frequency band edge
f1 = k1*fs/N           % quantized band-edge frequencies
f2 = k2*fs/N

w = kaiser(M,beta)'; % Kaiser window in "linear phase form"
H = [ ([0:k1-2]/(k1-1)).^8,ones(1,k2-k1+1),...
  ([k1-2:-1:0]/(k1-1)).^8, zeros(1,N/2-1)];
h = ifft(H); % desired impulse response
hodd = imag(h(1:2:N));     % This should be zero
ierr = norm(hodd)/norm(h); % Look at numerical round-off error
% Typical value: ierr = 4.1958e-15
% Also look at time aliasing:
aerr = norm(h(N/2-N/32:N/2+N/32))/norm(h);
% Typical value: 4.8300e-04
% put window in zero-phase form:

wzp = [w((M+1)/2:M), zeros(1,N-M), w(1:(M-1)/2)];
hw = wzp .* h; % single-sideband FIR filter, zero-centered
Hw = fft(hw);  % for results display: plot(db(Hw));

hh = [hw(N-(M-1)/2+1:N),hw(1:(M+1)/2)]; % caual FIR

hri = firpm(M-1, [f1,f2]/fn, [1,1], [1], 'Hilbert');
plot(db(fft([hri,zeros(1,N-M)]))); % freq resp plot

tic; % remember the current time
hrm = firpm(M-1, [0,(f2-fs/4)/fn,0.5,1], [1,1,0,0], [1,10]);
dt = toc; % design time dt can be minutes
hr = hrm .* j .^ [0:M-1]; % modulate lowpass to single-sideband

