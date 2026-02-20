%% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Read and Write from a serial port
%% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
close all ; clear all ; clc

% define port
portName = serialportlist("available");
portName = '/dev/ttyACM0' ;

% open port
fp = serialport(portName,115200)

% settings
sizeBuffer = 1 ;
vecSamples = 0 ;

v_motor = 0 ;
p_encoder = 0 ;


count = 0
%%
while(true)
    tic
    % find the first '\n' to know where to start to read
    while(vecSamples~=10)
        vecSamples = read( fp , 1 , "uint8" ) ;
    end

    % read serial port
    vecSamples = read( fp , 10*sizeBuffer , "uint8" ); % try to read 5, because each number is "XXXX\n"

    % take the serial port data and convert to a vector
    vec_samples_aux = str2num(char(vecSamples));
    vecSamples = vecSamples(end); % if it is \n, it keeps reading, if not will go to the first while of this loop

    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    vec_samples_aux

    if (isempty(vec_samples_aux)==0)

        v_motor = vec_samples_aux(:,1);
        p_encoder = vec_samples_aux(:,2);

        count = count+1;

        if(mod(count,5)==1)
            v_motor = count ;
            str_aux = sprintf('%04d,0000\n',v_motor);
            write(fp,str_aux,"uint8")
        end
    end
    toc


end


fclose(fp);




