% ArduinoTimerError.m
function ArduinoTimerError(~, event)
    fprintf('Arduino timer error: %s\n', event.Data.message);
end
