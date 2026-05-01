%% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% ECEN4532 - DSP Lab (Module 02)
% Osciloscope Mode - Talles Santos 2/19/26
% Rotine that read serial port data and plot in real time

%% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
close all ; clear all ; clc

% define port; check whatever port the Arduino is connected to
% and uncomment that one while commenting the other ports
portName = serialportlist("available");
portName = '/dev/ttyACM0' ;
% portName = '/dev/ttyACM1' ;
% portName = '/dev/ttyUSB0' ;

% open port
fp = serialport(portName,115200);

% settings
nSamplesBuffer = 20e3 ;   % nSamples of the circular buffer in the matlab script

% initialing variables
CircularBuffer = zeros(nSamplesBuffer,1);
count=1;

pause(2)
writeline(fp, "#10000");

%%
figure
while(true)

    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    % SAFE SERIAL READ (FIXED)
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    % vec_samples_aux = zeros(sizeBufferArduino,1);
    line = readline(fp)         % guarantees full line ending in \n
    val = str2double(line)    ;    % safer than str2num


    CircularBuffer(count) = val;
    %dydx = gradient(CircularBuffer, 0.1);
    dydx = CircularBuffer;
    count = mod(count, nSamplesBuffer) + 1;
    %dydx(abs(dydx)>5e3)=0;

    if( mod(count,1000) == 1)
        plot(dydx)
        drawnow
    end

end

fclose(fp);


