% ArduinoStop.m
% Run this to cleanly stop the background timer and close the serial port
% Call this after stopping the Simulink simulation

function ArduinoStop()
    global ArduinoTimer ArduinoPort

    % Stop and delete timer
    if ~isempty(ArduinoTimer)
        try
            stop(ArduinoTimer);
            delete(ArduinoTimer);
            fprintf('Arduino timer stopped.\n');
        catch
        end
        ArduinoTimer = [];
    end

    % Send zero voltage and close port
    if ~isempty(ArduinoPort) && isvalid(ArduinoPort)
        try
            writeline(ArduinoPort, '#+000000');
            pause(0.05);
            delete(ArduinoPort);
            fprintf('Arduino serial port closed.\n');
        catch
        end
        ArduinoPort = [];
    end
end
