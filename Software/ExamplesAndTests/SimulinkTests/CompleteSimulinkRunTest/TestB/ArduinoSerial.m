classdef ArduinoSerial < matlab.System
    properties (Access = private)
        SerialPort
    end

    properties (Nontunable)
        % Serial settings
        COMPort  = '/dev/ttyACM0'   % COM Port
        BaudRate = 115200           % Baud Rate

        % Encoder limit — shown as editable field in block dialog
        TargetPos (1,1) double = 500  % Encoder count limit (0 = disabled)

        % Direction dropdown — shown as dropdown in block dialog
        % Selects which direction triggers the limit
        Direction = 'Positive'        % Limit direction
    end

    properties (Hidden, Constant)
        % Defines the dropdown options for Direction
        DirectionSet = matlab.system.StringSet({'Positive', 'Negative', 'Both'})
    end

    methods (Access = protected)

        function setupImpl(obj)
            obj.SerialPort = serialport(obj.COMPort, obj.BaudRate);
            obj.SerialPort.Timeout = 0.05;
            configureTerminator(obj.SerialPort, "LF");
            flush(obj.SerialPort);
        end

        % Input:   v_in      = voltage command (-6 to +6)
        % Outputs: enc_out   = encoder count
        %          v_out     = voltage being applied
        %          limit_out = 1 when limit reached, 0 otherwise
        function [enc_out, v_out, limit_out] = stepImpl(obj, v_in)
            persistent last_enc last_v last_limit t_last
            if isempty(last_enc),   last_enc   = 0; end
            if isempty(last_v),     last_v     = 0; end
            if isempty(last_limit), last_limit = 0; end
            if isempty(t_last),     t_last     = tic; end

            target_step = 0.010;

            try
                flush(obj.SerialPort);

                % Determine signed target to send based on Direction setting
                switch obj.Direction
                    case 'Positive'
                        target_to_send = int32(abs(obj.TargetPos));
                    case 'Negative'
                        target_to_send = int32(-abs(obj.TargetPos));
                    case 'Both'
                        % Send positive value — Arduino checks both directions
                        target_to_send = int32(abs(obj.TargetPos));
                end

                % Send: #voltage,target\n
                % TargetPos = 0 disables the limit on the Arduino side
                packet = sprintf('#%+07d,%+07d\n', int32(v_in), target_to_send);
                writeline(obj.SerialPort, packet);

                % Wait for response: #±XXXXXX,±XXXXXX:B\n = 19 bytes
                timeout = 0.009;
                t_start = tic;
                while obj.SerialPort.NumBytesAvailable < 19
                    if toc(t_start) > timeout
                        break;
                    end
                end

                if obj.SerialPort.NumBytesAvailable >= 19
                    raw = readline(obj.SerialPort);
                    str = strtrim(char(raw));

                    colon_idx = strfind(str, ':');
                    if ~isempty(colon_idx)
                        main_part  = str(1:colon_idx(1)-1);
                        limit_part = str(colon_idx(1)+1:end);
                        vals = sscanf(main_part, '#%d,%d');
                        lim  = str2double(limit_part);
                        if numel(vals) == 2 && ~isnan(lim)
                            last_enc   = double(vals(1));
                            last_v     = double(vals(2));
                            last_limit = double(lim);
                        end
                    end
                end

                elapsed   = toc(t_last);
                remaining = target_step - elapsed;
                if remaining > 0.001
                    pause(remaining);
                end
                t_last = tic;

            catch
                t_last = tic;
            end

            enc_out   = last_enc;
            v_out     = last_v;
            limit_out = last_limit;
        end

        function releaseImpl(obj)
            delete(obj.SerialPort);
        end

        function [sz1, sz2, sz3] = getOutputSizeImpl(~)
            sz1 = [1 1]; sz2 = [1 1]; sz3 = [1 1];
        end

        function [dt1, dt2, dt3] = getOutputDataTypeImpl(~)
            dt1 = 'double'; dt2 = 'double'; dt3 = 'double';
        end

        function [c1, c2, c3] = isOutputComplexImpl(~)
            c1 = false; c2 = false; c3 = false;
        end

        function [f1, f2, f3] = isOutputFixedSizeImpl(~)
            f1 = true; f2 = true; f3 = true;
        end
    end
end
