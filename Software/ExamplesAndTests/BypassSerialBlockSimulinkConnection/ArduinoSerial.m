classdef ArduinoSerial < matlab.System
    properties (Access = private)
        SerialPort
    end

    properties (Nontunable)
        COMPort = '/dev/ttyACM0'      % COM Port
        BaudRate = 115200
    end

    methods (Access = protected)

        function setupImpl(obj)
            obj.SerialPort = serialport(obj.COMPort, obj.BaudRate);
            obj.SerialPort.Timeout = 0.05;  % 50ms timeout
            configureTerminator(obj.SerialPort, "LF");
            flush(obj.SerialPort);          % clear any garbage on startup
        end

        function [v_out, p_out] = stepImpl(obj, v_in, p_in)
            persistent last_v last_p t_last
            if isempty(last_v), last_v = 0; end
            if isempty(last_p), last_p = 0; end
            if isempty(t_last), t_last = tic; end

            target_step = 0.010;

            try
                % Flush anything leftover before sending
                flush(obj.SerialPort);

                % Send request
                packet = sprintf('#%+07d,%+07d\n', int32(v_in), int32(p_in));
                writeline(obj.SerialPort, packet);

                % Wait for exactly one response
                timeout = 0.009;
                t_start = tic;
                while obj.SerialPort.NumBytesAvailable < 17
                    if toc(t_start) > timeout
                        break;
                    end
                end

                % Read exactly one line
                if obj.SerialPort.NumBytesAvailable >= 17
                    line = readline(obj.SerialPort);
                    str = strtrim(char(line));
                    vals = sscanf(str, '#%d,%d');
                    if numel(vals) == 2
                        last_v = double(vals(1));
                        last_p = double(vals(2));
                    end
                end

                % Pace to 10ms
                elapsed = toc(t_last);
                remaining = target_step - elapsed;
                if remaining > 0.001
                    pause(remaining);
                end
                t_last = tic;

            catch
                t_last = tic;
            end

            v_out = last_v;
            p_out = last_p;
        end

        function releaseImpl(obj)
            delete(obj.SerialPort);
        end

        function [sz1, sz2] = getOutputSizeImpl(~)
            sz1 = [1 1];
            sz2 = [1 1];
        end

        function [dt1, dt2] = getOutputDataTypeImpl(~)
            dt1 = 'double';
            dt2 = 'double';
        end

        function [c1, c2] = isOutputComplexImpl(~)
            c1 = false;
            c2 = false;
        end

        function [f1, f2] = isOutputFixedSizeImpl(~)
            f1 = true;
            f2 = true;
        end
    end
end
