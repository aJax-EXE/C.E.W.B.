classdef ArduinoSerial < matlab.System

    properties (Nontunable)
        COMPort     = '/dev/ttyACM0'  % COM Port
        BaudRate    = 115200          % Baud Rate
        TimerPeriod = 0.005           % Background timer period (s) — 0.005 = 200Hz
    end

    properties (Access = private)
        SerialPort
        BgTimer
        LatestEnc = 0   % latest encoder count from Arduino
        LatestV   = 0   % latest voltage readback from Arduino
        VCmd      = 0   % voltage command to send to Arduino
    end

    methods (Access = protected)

        function setupImpl(obj)
            % Open serial port
            obj.SerialPort = serialport(obj.COMPort, obj.BaudRate);
            obj.SerialPort.Timeout = 0.002;
            configureTerminator(obj.SerialPort, "LF");
            pause(2);
            flush(obj.SerialPort);

            % Start background timer — runs independently of Simulink step
            obj.BgTimer = timer(...
                'ExecutionMode', 'fixedRate', ...
                'Period',        obj.TimerPeriod, ...
                'TimerFcn',      @(~,~) obj.timerCallback(), ...
                'Name',          'ArduinoSerialTimer');
            start(obj.BgTimer);
            fprintf('ArduinoSerial: background timer started at %.0f Hz\n', ...
                    1/obj.TimerPeriod);
        end

        % Input:   v_in    = voltage command (-6 to +6)
        % Outputs: enc_out = encoder count (raw)
        %          v_out   = voltage being applied (for scope)
        function [enc_out, v_out] = stepImpl(obj, v_in)
            % Write voltage command for background timer to pick up
            obj.VCmd = v_in;

            % Read latest values updated by background timer
            enc_out = obj.LatestEnc;
            v_out   = obj.LatestV;
        end

        function releaseImpl(obj)
            % Stop timer first
            if ~isempty(obj.BgTimer) && isvalid(obj.BgTimer)
                stop(obj.BgTimer);
                delete(obj.BgTimer);
            end

            % Send zero voltage then close port
            if ~isempty(obj.SerialPort) && isvalid(obj.SerialPort)
                try
                    writeline(obj.SerialPort, '#+000000');
                    pause(0.05);
                catch
                end
                delete(obj.SerialPort);
            end
            fprintf('ArduinoSerial: stopped and port closed.\n');
        end

        function [sz1, sz2] = getOutputSizeImpl(~)
            sz1 = [1 1]; sz2 = [1 1];
        end

        function [dt1, dt2] = getOutputDataTypeImpl(~)
            dt1 = 'double'; dt2 = 'double';
        end

        function [c1, c2] = isOutputComplexImpl(~)
            c1 = false; c2 = false;
        end

        function [f1, f2] = isOutputFixedSizeImpl(~)
            f1 = true; f2 = true;
        end

    end

    methods (Access = private)

        function timerCallback(obj)
            % Runs at 200Hz in background independently of Simulink
            % Sends voltage command and reads encoder response

            if isempty(obj.SerialPort) || ~isvalid(obj.SerialPort)
                return;
            end

            try
                % Flush stale data
                flush(obj.SerialPort);

                % Send latest voltage command scaled by 10000
                v_scaled = int32(obj.VCmd * 10000);
                writeline(obj.SerialPort, sprintf('#%+07d', v_scaled));

                % Wait briefly for response — non-blocking 3ms max
                t_start = tic;
                while obj.SerialPort.NumBytesAvailable < 17
                    if toc(t_start) > 0.003
                        return;  % no response yet — try next cycle
                    end
                end

                % Read and parse response: #sXXXXXX,sXXXXXX\n
                raw  = readline(obj.SerialPort);
                str  = strtrim(char(raw));
                vals = sscanf(str, '#%d,%d');

                if numel(vals) == 2
                    obj.LatestEnc = double(vals(1));
                    obj.LatestV   = double(vals(2)) / 10000.0;
                end

            catch
                % silently ignore — timer keeps running
            end
        end

    end
end