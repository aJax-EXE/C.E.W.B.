classdef ArduinoSerial_QubeStyle < matlab.System

    methods (Access = protected)

        function setupImpl(~)
            % Serial communication is handled by the background timer
            % Run ArduinoBackground.m in the MATLAB command window BEFORE
            % starting this simulation
            global ArduinoData
            if isempty(ArduinoData)
                error('ArduinoData not found. Run ArduinoBackground.m first.');
            end
            fprintf('ArduinoSerial block ready — using background timer data.\n');
        end

        % Input:   v_in    = voltage command from Simulink (-6 to +6)
        % Outputs: enc_out = encoder count
        %          v_out   = voltage being applied
        function [enc_out, v_out] = stepImpl(~, v_in)
            global ArduinoData

            % Write voltage command for background timer to pick up
            ArduinoData.v_cmd = v_in;

            % Read latest values from background timer
            enc_out = ArduinoData.enc;
            v_out   = ArduinoData.v;
        end

        function releaseImpl(~)
            % Send zero voltage via global on stop
            global ArduinoData
            if ~isempty(ArduinoData)
                ArduinoData.v_cmd = 0;
            end
            fprintf('ArduinoSerial block released. Run ArduinoStop to close port.\n');
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
