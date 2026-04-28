classdef ArduinoSerial < matlab.System
    properties (Access = private)
        SerialPort
    end

    properties (Nontunable)
        COMPort  = '/dev/ttyACM0'   % COM Port
        BaudRate = 115200           % Baud Rate
    end

    methods (Access = protected)

        function setupImpl(obj)
            obj.SerialPort = serialport(obj.COMPort, obj.BaudRate);
            obj.SerialPort.Timeout = 0.05;
            configureTerminator(obj.SerialPort, "LF");
            pause(2);              % wait for Arduino to finish booting
            flush(obj.SerialPort); % clear all boot messages at once
        end

        % Input:   v_in    = voltage command from Simulink (-6 to +6)
        % Outputs: enc_out = encoder count
        %          v_out   = voltage being applied (for scope)
        function [enc_out, v_out] = stepImpl(obj, v_in)
            persistent last_enc last_v t_last
            if isempty(last_enc), last_enc = 0; end
            if isempty(last_v),   last_v   = 0; end
            if isempty(t_last),   t_last   = tic; end

            target_step = 0.020;  % 20ms step

            try
                flush(obj.SerialPort);

                % Scale voltage to integer: v_in x 10000
                % e.g.  3.75V -> #+037500
                %       -3.75V -> #-037500
                v_scaled = int32(v_in * 10000);
                packet   = sprintf('#%+07d\n', v_scaled);
                writeline(obj.SerialPort, packet);

                % Wait for response: #sXXXXXX,sXXXXXX\n = 17 bytes
                timeout = 0.018;
                t_start = tic;
                while obj.SerialPort.NumBytesAvailable < 17
                    if toc(t_start) > timeout
                        break;
                    end
                end

                % Read and parse response
                if obj.SerialPort.NumBytesAvailable >= 17
                    raw = readline(obj.SerialPort);
                    str = strtrim(char(raw));
                    vals = sscanf(str, '#%d,%d');
                    if numel(vals) == 2
                        last_enc = double(vals(1));
                        % Descale voltage: integer / 10000 = real volts
                        last_v   = double(vals(2)) / 10000.0;
                    end
                end

                % Pace to 20ms
                elapsed   = toc(t_last);
                remaining = target_step - elapsed;
                if remaining > 0.001
                    pause(remaining);
                end
                t_last = tic;

            catch
                t_last = tic;
            end

            enc_out = last_enc;
            v_out   = last_v;
        end

        function releaseImpl(obj)
            try
                % Send zero voltage before closing — stops motor cleanly
                writeline(obj.SerialPort, '#+000000');
                pause(0.05);
            catch
            end
            delete(obj.SerialPort);
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
end
