classdef ArduinoSerial < matlab.System
    properties (Access = private)
        SerialPort
    end

    properties (Nontunable)
        COMPort  = '/dev/ttyACM0'   % COM Port
        BaudRate = 115200
    end

    methods (Access = protected)

        function setupImpl(obj)
            obj.SerialPort = serialport(obj.COMPort, obj.BaudRate);
            obj.SerialPort.Timeout = 0.05;
            configureTerminator(obj.SerialPort, "LF");
            flush(obj.SerialPort);
        end

        % No inputs
        % Output: enc_out = current encoder count from Arduino
        function enc_out = stepImpl(obj)
            persistent last_enc t_last
            if isempty(last_enc), last_enc = 0; end
            if isempty(t_last),   t_last   = tic; end

            target_step = 0.010;

            try
                flush(obj.SerialPort);

                % Send a single trigger byte to request encoder reading
                write(obj.SerialPort, uint8('R'), 'uint8');

                % Wait for response: #±XXXXXX,±XXXXXX:B\n = 19 bytes
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

                    % Split on ':' to get main part and limit bit
                    colon_idx = strfind(str, ':');

                    if ~isempty(colon_idx)
                        main_part = str(1:colon_idx(1)-1);  % #±XXXXXX,±XXXXXX
                        vals = sscanf(main_part, '#%d,%d');
                        if numel(vals) >= 1
                            last_enc = double(vals(1));
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
        end

        function releaseImpl(obj)
            delete(obj.SerialPort);
        end

        % Single scalar double output
        function sz1 = getOutputSizeImpl(~)
            sz1 = [1 1];
        end

        function dt1 = getOutputDataTypeImpl(~)
            dt1 = 'double';
        end

        function c1 = isOutputComplexImpl(~)
            c1 = false;
        end

        function f1 = isOutputFixedSizeImpl(~)
            f1 = true;
        end
    end
end