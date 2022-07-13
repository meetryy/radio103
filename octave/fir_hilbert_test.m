 M = 257;    % window length = FIR filter length (Window Method)
  fs = 22050; % sampling rate assumed (Hz)
  f1 = 530;   % lower pass-band limit = transition bandwidth (Hz)
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
 plot(db(fft([hh,zeros(1,N-M)]))); % freq resp plot


