% ArduinoBackground.m
% Run this script BEFORE starting the Simulink simulation
% It starts a background timer that continuously reads the Arduino
% and stores the latest values in global variables

global ArduinoData ArduinoPort ArduinoTimer

% ── Config ────────────────────────────────────────────────
COM_PORT  = '/dev/ttyACM0';
BAUD_RATE = 115200;
TIMER_PERIOD = 0.005;  % 5ms = 200Hz update rate
% ──────────────────────────────────────────────────────────

% Stop any existing timer/port first
ArduinoStop;

% Shared data structure — Simulink reads from this
ArduinoData.enc    = 0;   % latest encoder count
ArduinoData.v      = 0;   % latest voltage readback
ArduinoData.v_cmd  = 0;   % voltage command FROM Simulink TO Arduino

% Open serial port
ArduinoPort = serialport(COM_PORT, BAUD_RATE);
ArduinoPort.Timeout = 0.002;  % 2ms timeout — non-blocking
configureTerminator(ArduinoPort, "LF");
pause(2);
flush(ArduinoPort);

fprintf('Arduino serial port open at %s\n', COM_PORT);

% Start background timer
ArduinoTimer = timer(...
    'ExecutionMode', 'fixedRate', ...
    'Period',        TIMER_PERIOD, ...
    'TimerFcn',      @ArduinoTimerCallback, ...
    'ErrorFcn',      @ArduinoTimerError, ...
    'Name',          'ArduinoTimer');

start(ArduinoTimer);
fprintf('Background timer started at %.0f Hz\n', 1/TIMER_PERIOD);
fprintf('Run your Simulink model now.\n');
