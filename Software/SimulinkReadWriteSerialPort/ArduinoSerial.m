classdef ArduinoSerial < matlab.System
    properties (Access = private)
        SerialPort
    end
    
    properties (Nontunable)
        COMPort = '/dev/ttyACM0'      % change to your port
        BaudRate = 115200
    end
    
    methods (Access = protected)
        function setupImpl(obj)
            obj.SerialPort = serialport(obj.COMPort, obj.BaudRate);
            obj.SerialPort.Timeout = 0.01;
            configureTerminator(obj.SerialPort, "LF");
            flush(obj.SerialPort);
        end
        
        function [v_out, p_out] = stepImpl(obj, v_in, p_in)
            % --- Send to Arduino ---
            packet = sprintf('#%+07d,%+07d\n', int32(v_in), int32(p_in));
            writeline(obj.SerialPort, packet);
            
            % --- Receive from Arduino ---
            v_out = double(0);
            p_out = double(0);
            
            if obj.SerialPort.NumBytesAvailable > 0
                try
                    line = readline(obj.SerialPort);
                    vals = sscanf(char(line), '#%d,%d');
                    if numel(vals) == 2
                        v_out = double(vals(1));
                        p_out = double(vals(2));
                    end
                catch
                    % discard bad packet, keep last values
                end
            end
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
