% ArduinoTimerCallback.m
% Called automatically by the background timer every 5ms
% Sends voltage command and reads encoder response from Arduino

function ArduinoTimerCallback(~, ~)
    global ArduinoData ArduinoPort

    if isempty(ArduinoPort) || ~isvalid(ArduinoPort)
        return;
    end

    try
        % Flush stale data
        flush(ArduinoPort);

        % Send latest voltage command from Simulink
        v_scaled = int32(ArduinoData.v_cmd * 10000);
        writeline(ArduinoPort, sprintf('#%+07d', v_scaled));

        % Wait briefly for response (non-blocking — 2ms timeout)
        t = tic;
        while ArduinoPort.NumBytesAvailable < 17
            if toc(t) > 0.003  % 3ms max wait
                return;        % no response yet — try next cycle
            end
        end

        % Read and parse response
        raw  = readline(ArduinoPort);
        str  = strtrim(char(raw));
        vals = sscanf(str, '#%d,%d');

        if numel(vals) == 2
            ArduinoData.enc = double(vals(1));
            ArduinoData.v   = double(vals(2)) / 10000.0;
        end

    catch
        % silently ignore errors — timer keeps running
    end
end
