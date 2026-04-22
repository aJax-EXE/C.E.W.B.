classdef ArduinoSerialTestC < matlab.System
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
            flush(obj.SerialPort);
        end
 
        % Input:   v_in    = voltage command from Simulink controller (-6 to +6)
        % Outputs: enc_out = encoder count (feed back into controller)
        %          v_out   = voltage being applied (for scope)
        function [enc_out, v_out] = stepImpl(obj, v_in)
            persistent last_enc last_v t_last
            if isempty(last_enc), last_enc = 0; end
            if isempty(last_v),   last_v   = 0; end
            if isempty(t_last),   t_last   = tic; end
 
            target_step = 0.010;
 
            try
                flush(obj.SerialPort);
 
                % Send voltage command: #±XXXXXXX\n
                packet = sprintf('#%+07d\n', int32(v_in));
                writeline(obj.SerialPort, packet);
 
                % Wait for response: #±XXXXXX,±XXXXXX:0\n = 19 bytes
                timeout = 0.009;
                t_start = tic;
                while obj.SerialPort.NumBytesAvailable < 19
                    if toc(t_start) > timeout
                        break;
                    end
                end
 
                % Read and parse response
                if obj.SerialPort.NumBytesAvailable >= 19
                    raw = readline(obj.SerialPort);
                    str = strtrim(char(raw));
 
                    % Split on ':' — we only need the main part
                    colon_idx = strfind(str, ':');
                    if ~isempty(colon_idx)
                        main_part = str(1:colon_idx(1)-1);
                        vals = sscanf(main_part, '#%d,%d');
                        if numel(vals) == 2
                            last_enc = double(vals(1));
                            last_v   = double(vals(2));
                        end
                    end
                end
 
                % Pace to 10ms
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
                % Send zero voltage before closing — stops the motor cleanly
                writeline(obj.SerialPort, '#-000000');
                pause(0.02);  % give Arduino time to receive and act on it
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

