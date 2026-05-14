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
nSamplesBuffer = 5e3 ;   % nSamples of the circular buffer in the matlab script

% initialing variables
CircularBuffer = zeros(nSamplesBuffer,2);
dvdt = zeros(nSamplesBuffer,1);
count=1;

pause(2)
writeline(fp, "#15000");
pause(5)

%%
figure()
while(true)

    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    % SAFE SERIAL READ (FIXED)
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    % vec_samples_aux = zeros(sizeBufferArduino,1);
    line = readline(fp)         % guarantees full line ending in \n

    parts = split(line, ',');

    if(size(parts,1)==2)
        
        firstPart = parts(1); % Result: "+001160"
        secondPart = parts(2); % Result: "2650"


        CircularBuffer(count,1) = str2double(firstPart);
        CircularBuffer(count,2) = str2double(secondPart);
        count = mod(count, nSamplesBuffer) + 1

        % dvdt = diff(CircularBuffer(:,2)) ./ diff(CircularBuffer(:,1));

        if( mod(count,100) == 0)
            subplot(1,2,1)
            disp('entrou!!!')
            plot(CircularBuffer(:,1),'b'); hold on;
            plot(CircularBuffer(:,2)/255 * 20,'r'); 
            ylim([-20 20])
            

            subplot(1,2,2)
            plot(CircularBuffer(:,2),'.-')
            ylim([-260 260])
            % plot(dvdt,'.-')
            drawnow
        end
    end

end

fclose(fp);


