classdef MCAN_INTERRUPT_STATUS < matlab.System & ...
        coder.ExternalDependency
    %
    % System object template for a source block.
    % 
    % This template includes most, but not all, possible properties,
    % attributes, and methods that you can implement for a System object in
    % Simulink.
    %
    % NOTE: When renaming the class name Source, the file name and
    % constructor name must be updated to use the class name.
    %
    
    
    %#codegen
    %#ok<*EMCA>
    
    properties
        % Public, tunable properties.
    end
    
    properties (Nontunable)
        % Public, non-tunable properties.

        %MCAN Instance
        MCAN_Instance = 'MCAN0';
        %All interrupts
        INT_STATUS_ALL (1,1) logical = false;
        %RX FIFO 0 new message
        RX_FIFO0_newmsg (1,1) logical = false;
        %RX FIFO 0 watermark reached
        RX_FIFO0_wtrmark (1,1) logical = false;
        %RX FIFO 0 full
        RX_FIFO0_full (1,1) logical = false;
        %RX FIFO 0 message lost
        RX_FIFIO0_msg_lost (1,1) logical = false;
        %RX FIFO 1 new message
        RX_FIFO1_newmsg (1,1) logical = false;
        %RX FIFO 1 watermark reached
        RX_FIFO1_wtrmark (1,1) logical = false;
        %RX FIFO 1 full
        RX_FIFO1_full (1,1) logical = false;
        %RX FIFO 1 message lost
        RX_FIFIO1_msg_lost (1,1) logical = false;
        %High priority message
        Highprt_msg (1,1) logical = false;
        %Transmission complete
        TX_complete (1,1) logical = false;
        %Transmission cancellation finished
        TX_cancellation_finished (1,1) logical = false;
        %TX FIFO empty
        TXFIFIO_empty (1,1) logical = false;
        %TX Event FIFO new entry
        TXEventFIFO_newentry (1,1) logical = false;
        %TX Event FIFO watermark reached
        TXEventFIFO_watermark (1,1) logical = false;
        %TX Event FIFO Full
        TXEventFIFO_full (1,1) logical = false;
        %TX Event FIFO element lost
        TXEventFIFO_elemntlost (1,1) logical = false;
        %Timestamp wraparound
        Timestamp_wrparound (1,1) logical = false;
        %Message RAM Access failure
        MsgRAM_accessfail (1,1) logical = false;
        %Timeout occured
        Timeout_occured (1,1) logical = false;
        %Message stored to dedicated receive buffer 
        Msg_stored_dedrxbuf (1,1) logical = false;
        %Bit error corrected
        Bit_error_corrected (1,1) logical = false;
        %Bit error uncorrected
        Bit_error_uncorrected (1,1) logical = false;
        %Error logging overflow
        Error_logging_overflow (1,1) logical = false;
        %Eroor Passive
        Error_passive (1,1) logical = false;
        %Warning status
        Warning_status (1,1) logical = false;
        %Bus off status
        Busoff_status (1,1) logical = false;
        %Watchdog interrupt
        Watchdog_intr (1,1) logical = false;
        %Protocol error in arbitration phase
        Protocol_err_arbtrphase (1,1) logical = false;
        %Protocol error in data phase
        Protocol_err_dataphase (1,1) logical = false;
        %Access to reserved address
        Accs_reservedaddr (1,1) logical = false;

    end

    properties (Constant, Hidden)
        MCAN_InstanceSet = matlab.system.StringSet({'MCAN0', 'MCAN1', 'MCAN2', 'MCAN3'});
       
    end
    
    properties (Dependent, Access=protected)
        MCAN_InstanceEnum;  
        
    end
    properties (Access = private)
        % Pre-computed constants.
        baseaddr = 0;
        outputportcnt = 0;
    end
    
    methods
        % Constructor
        function obj = MCAN_INTERRUPT_STATUS(varargin)
            % Support name-value pair arguments when constructing the object.
            setProperties(obj,nargin,varargin{:});
        end

        function ret = get.MCAN_InstanceEnum(obj)
            if(isequal(obj.MCAN_Instance, 'MCAN0'))
                ret = uint32(0);
            elseif(isequal(obj.MCAN_Instance, 'MCAN1'))
                ret = uint32(1);
            elseif(isequal(obj.MCAN_Instance, 'MCAN2'))
                ret = uint32(2);
            elseif(isequal(obj.MCAN_Instance, 'MCAN3'))
                ret = uint32(3);
            end
        end

        


    end
    
    methods (Access=protected)
        function setupImpl(obj) %#ok<MANU>
            if isempty(coder.target)
                % Place simulation setup code here
            else
                % Call C-function implementing device initialization
                % coder.cinclude('source.h');
                % coder.ceval('source_init');
                coder.cinclude('MW_CAN.h');
                switch uint16(obj.MCAN_InstanceEnum)
                    case 0
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_MCAN0_MSG_RAM_U_BASE');
                    case 1
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_MCAN1_MSG_RAM_U_BASE');
                    case 2
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_MCAN2_MSG_RAM_U_BASE');
                    case 3
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_MCAN3_MSG_RAM_U_BASE');
                    otherwise
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_MCAN0_MSG_RAM_U_BASE');
                end

            if(obj.INT_STATUS_ALL)
               obj.outputportcnt = obj.outputportcnt + 1;
            end
            if(obj.RX_FIFO0_newmsg)
                obj.outputportcnt = obj.outputportcnt + 1;
            end
            if(obj.RX_FIFO0_wtrmark)
                obj.outputportcnt = obj.outputportcnt + 1;
            end
            if(obj.RX_FIFO0_full)
                obj.outputportcnt = obj.outputportcnt + 1;
            end
            if(obj.RX_FIFIO0_msg_lost)
                obj.outputportcnt = obj.outputportcnt + 1;
            end
            if(obj.RX_FIFO1_newmsg)
                obj.outputportcnt = obj.outputportcnt + 1;
            end
            if(obj.RX_FIFO1_wtrmark)
                obj.outputportcnt = obj.outputportcnt + 1;
            end
            if(obj.RX_FIFO1_full)
                obj.outputportcnt = obj.outputportcnt + 1;
            end
            if(obj.RX_FIFIO1_msg_lost)
                obj.outputportcnt = obj.outputportcnt + 1;
            end
            if(obj.Highprt_msg)
                obj.outputportcnt = obj.outputportcnt + 1;
            end
            if(obj.TX_complete)
                obj.outputportcnt = obj.outputportcnt + 1;
            end
            if(obj.TX_cancellation_finished)
                obj.outputportcnt = obj.outputportcnt + 1;
            end
            if(obj.TXFIFIO_empty)
                obj.outputportcnt = obj.outputportcnt + 1;
            end
            if(obj.TXEventFIFO_newentry)
                obj.outputportcnt = obj.outputportcnt + 1;
            end
            if(obj.TXEventFIFO_watermark)
                obj.outputportcnt = obj.outputportcnt + 1;
            end
            if(obj.TXEventFIFO_full)
                obj.outputportcnt = obj.outputportcnt + 1;
            end
            if(obj.TXEventFIFO_elemntlost)
                obj.outputportcnt = obj.outputportcnt + 1;
            end
            if(obj.Timestamp_wrparound)
                obj.outputportcnt = obj.outputportcnt + 1;
            end
            if(obj.MsgRAM_accessfail)
                obj.outputportcnt = obj.outputportcnt + 1;
            end
            if(obj.Timeout_occured)
                obj.outputportcnt = obj.outputportcnt + 1;
            end
            if(obj.Msg_stored_dedrxbuf)
                obj.outputportcnt = obj.outputportcnt + 1;
            end
            if(obj.Bit_error_corrected)
                obj.outputportcnt = obj.outputportcnt + 1;
            end
            if(obj.Bit_error_uncorrected)
                obj.outputportcnt = obj.outputportcnt + 1;
            end
            if(obj.Error_logging_overflow)
                obj.outputportcnt = obj.outputportcnt + 1;
            end
            if(obj.Error_passive)
                obj.outputportcnt = obj.outputportcnt + 1;
            end
            if(obj.Warning_status)
                obj.outputportcnt = obj.outputportcnt + 1;
            end
            if(obj.Busoff_status)
                obj.outputportcnt = obj.outputportcnt + 1;
            end
            if(obj.Watchdog_intr)
                obj.outputportcnt = obj.outputportcnt + 1;
            end
            if(obj.Protocol_err_arbtrphase)
                obj.outputportcnt = obj.outputportcnt + 1;
            end
            if(obj.Protocol_err_dataphase)
                obj.outputportcnt = obj.outputportcnt + 1;
            end
            if(obj.Accs_reservedaddr)
                obj.outputportcnt = obj.outputportcnt + 1;
            end

            

                
            end
        end
        
        function [IS,varargout] = stepImpl(obj)   %#ok<MANU>
            
            if isempty(coder.target)
                % Place simulation output code here
            else
                % Call C-function implementing device output
                %y = coder.ceval('source_output');
                

                x = zeros(1,30,'logical');
                y = uint32(0);
                can_intstatus_step_struct = struct('base', obj.baseaddr, 'instance', uint32(obj.MCAN_InstanceEnum), 'Interrupt_status', uint32(obj.INT_STATUS_ALL),'RXFIFO0newmsg', uint32(obj.RX_FIFO0_newmsg), 'RXFIFO0wtrmark', uint32(obj.RX_FIFO0_wtrmark), 'RXFIFO0full', uint32(obj.RX_FIFO0_full) , 'RXFIFO0msglost', uint32(obj.RX_FIFIO0_msg_lost), 'RXFIFO1newmsg', uint32(obj.RX_FIFO1_newmsg), 'RXFIFO1wtrmark', uint32(obj.RX_FIFO1_wtrmark), 'RXFIFO1full', uint32(obj.RX_FIFO1_full), 'RXFIFO1msglost', uint32(obj.RX_FIFIO1_msg_lost), 'Highprioritymsg', uint32(obj.Highprt_msg), 'TXcomplete', uint32(obj.TX_complete), 'TXcancellationfin', uint32(obj.TX_cancellation_finished), 'TXFIFOempty', uint32(obj.TXFIFIO_empty), 'TXEventFIFOnewentry', uint32(obj.TXEventFIFO_newentry), 'TXEventFIFOwatermark', uint32(obj.TXEventFIFO_watermark), 'TXEventFIFOfull', uint32(obj.TXEventFIFO_full), 'TXEventFIFOelementlost', uint32(obj.TXEventFIFO_elemntlost), 'Timestampwraparound', uint32(obj.Timestamp_wrparound), 'MsgRAMacsfail', uint32(obj.MsgRAM_accessfail), 'Timeoutoccured', uint32(obj.Timeout_occured), 'MsgstrdedRxbuf', uint32(obj.Msg_stored_dedrxbuf), 'Biterrorcorr', uint32(obj.Bit_error_corrected), 'Biterruncorr', uint32(obj.Bit_error_uncorrected), 'Errorloggingovf', uint32(obj.Error_logging_overflow), 'Errorpassive', uint32(obj.Error_passive), 'Warningstatus', uint32(obj.Warning_status), 'Busoffstatus', uint32(obj.Busoff_status), 'Watchdogintr', uint32(obj.Watchdog_intr), 'Protocolerrarbitphase', uint32(obj.Protocol_err_arbtrphase), 'Protocolerrdataphase', uint32(obj.Protocol_err_dataphase), 'Accsresaddr', uint32(obj.Accs_reservedaddr));
                coder.cstructname(can_intstatus_step_struct, 'CANINTSTATUS_step_struct', 'extern', 'HeaderFile', 'MW_CAN.h');
                coder.ceval('CANINTSTATUS_step', coder.ref(can_intstatus_step_struct), coder.wref(x), coder.wref(y));
                index = 1;

                if(obj.INT_STATUS_ALL)
                    IS = y;
                else
                    IS = 0;
                end
                if(obj.RX_FIFO0_newmsg)
                    varargout{index} = x(index);
                    index = index + 1;
                end
                if(obj.RX_FIFO0_wtrmark)
                    varargout{index} = x(index);
                    index = index + 1;
                end
                if(obj.RX_FIFO0_full)
                    varargout{index} = x(index);
                    index = index + 1;
                end
                if(obj.RX_FIFIO0_msg_lost)
                    varargout{index} = x(index);
                    index = index + 1;
                end
                if(obj.RX_FIFO1_newmsg)
                    varargout{index} = x(index);
                    index = index + 1;
                end
                if(obj.RX_FIFO1_wtrmark)
                    varargout{index} = x(index);
                    index = index + 1;
                end
                if(obj.RX_FIFO1_full)
                    varargout{index} = x(index);
                    index = index + 1;
                end
                if(obj.RX_FIFIO1_msg_lost)
                    varargout{index} = x(index);
                    index = index + 1;
                end
                if(obj.Highprt_msg)
                    varargout{index} = x(index);
                    index = index + 1;
                end
                if(obj.TX_complete)
                    varargout{index} = x(index);
                    index = index + 1;
                end
                if(obj.TX_cancellation_finished)
                    varargout{index} = x(index);
                    index = index + 1;
                end
                if(obj.TXFIFIO_empty)
                    varargout{index} = x(index);
                    index = index + 1;
                end
                if(obj.TXEventFIFO_newentry)
                    varargout{index} = x(index);
                    index = index + 1;
                end
                if(obj.TXEventFIFO_watermark)
                    varargout{index} = x(index);
                    index = index + 1;
                end
                if(obj.TXEventFIFO_full)
                    varargout{index} = x(index);
                    index = index + 1;
                end
                if(obj.TXEventFIFO_elemntlost)
                    varargout{index} = x(index);
                    index = index + 1;
                end
                if(obj.Timestamp_wrparound)
                    varargout{index} = x(index);
                    index = index + 1;
                end
                if(obj.MsgRAM_accessfail)
                    varargout{index} = x(index);
                    index = index + 1;
                end
                if(obj.Timeout_occured)
                    varargout{index} = x(index);
                    index = index + 1;
                end
                if(obj.Msg_stored_dedrxbuf)
                    varargout{index} = x(index);
                    index = index + 1;
                end
                if(obj.Bit_error_corrected)
                    varargout{index} = x(index);
                    index = index + 1;
                end
                if(obj.Bit_error_uncorrected)
                    varargout{index} = x(index);
                    index = index + 1;
                end
                if(obj.Error_logging_overflow)
                    varargout{index} = x(index);
                    index = index + 1;
                end
                if(obj.Error_passive)
                    varargout{index} = x(index);
                    index = index + 1;
                end
                if(obj.Warning_status)
                    varargout{index} = x(index);
                    index = index + 1;
                end
                if(obj.Busoff_status)
                    varargout{index} = x(index);
                    index = index + 1;
                end
                if(obj.Watchdog_intr)
                    varargout{index} = x(index);
                    index = index + 1;
                end
                if(obj.Protocol_err_arbtrphase)
                    varargout{index} = x(index);
                    index = index + 1;
                end
                if(obj.Protocol_err_dataphase)
                    varargout{index} = x(index);
                    index = index + 1;
                end
                if(obj.Accs_reservedaddr)
                    varargout{index} = x(index);
                    index = index + 1;
                end
             
            end
        end
        
        function releaseImpl(obj) %#ok<MANU>
            if isempty(coder.target)
                % Place simulation termination code here
            else
                % Call C-function implementing device termination
                %coder.ceval('source_terminate');
            end
        end
    end
    
    methods (Access=protected)
        %% Define output properties
        function num = getNumInputsImpl(~)
            num = 0;
        end
        
        function num = getNumOutputsImpl(obj)
            num = 0;
            if(obj.INT_STATUS_ALL)
                num = num + 1;
            end
            if(obj.RX_FIFO0_newmsg)
                num = num + 1;
            end
            if(obj.RX_FIFO0_wtrmark)
                num = num + 1;
            end
            if(obj.RX_FIFO0_full)
                num = num + 1;
            end
            if(obj.RX_FIFIO0_msg_lost)
                num = num + 1;
            end
            if(obj.RX_FIFO1_newmsg)
                num = num + 1;
            end
            if(obj.RX_FIFO1_wtrmark)
                num = num + 1;
            end
            if(obj.RX_FIFO1_full)
                num = num + 1;
            end
            if(obj.RX_FIFIO1_msg_lost)
                num = num + 1;
            end
            if(obj.Highprt_msg)
                num = num + 1;
            end
            if(obj.TX_complete)
                num = num + 1;
            end
            if(obj.TX_cancellation_finished)
                num = num + 1;
            end
            if(obj.TXFIFIO_empty)
                num = num + 1;
            end
            if(obj.TXEventFIFO_newentry)
                num = num + 1;
            end
            if(obj.TXEventFIFO_watermark)
                num = num + 1;
            end
            if(obj.TXEventFIFO_full)
                num = num + 1;
            end
            if(obj.TXEventFIFO_elemntlost)
                num = num + 1;
            end
            if(obj.Timestamp_wrparound)
                num = num + 1;
            end
            if(obj.MsgRAM_accessfail)
                num = num + 1;
            end
            if(obj.Timeout_occured)
                num = num + 1;
            end
            if(obj.Msg_stored_dedrxbuf)
                num = num + 1;
            end
            if(obj.Bit_error_corrected)
                num = num + 1;
            end
            if(obj.Bit_error_uncorrected)
                num = num + 1;
            end
            if(obj.Error_logging_overflow)
                num = num + 1;
            end
            if(obj.Error_passive)
                num = num + 1;
            end
            if(obj.Warning_status)
                num = num + 1;
            end
            if(obj.Busoff_status)
                num = num + 1;
            end
            if(obj.Watchdog_intr)
                num = num + 1;
            end
            if(obj.Protocol_err_arbtrphase)
                num = num + 1;
            end
            if(obj.Protocol_err_dataphase)
                num = num + 1;
            end
            if(obj.Accs_reservedaddr)
                num = num + 1;
            end
        end

        function varargout = getOutputNamesImpl(obj)
            index = 1;
            num = getNumOutputsImpl(obj);
            if num > 0
                varargout = cell(1,num);
                if(obj.INT_STATUS_ALL)
                    varargout{index} = 'IR';
                    index = index + 1;
                end
                if(obj.RX_FIFO0_newmsg)
                    varargout{index} = 'RF0NM';
                    index = index + 1;
                end
                if(obj.RX_FIFO0_wtrmark)
                    varargout{index} = 'RF0W';
                    index = index + 1;
                end
                if(obj.RX_FIFO0_full)
                    varargout{index} = 'RF0F';
                    index = index + 1;
                end
                if(obj.RX_FIFIO0_msg_lost)
                    varargout{index} = 'RF0ML';
                    index = index + 1;
                end
                if(obj.RX_FIFO1_newmsg)
                    varargout{index} = 'RF1NM';
                    index = index + 1;
                end
                if(obj.RX_FIFO1_wtrmark)
                    varargout{index} = 'RF1W';
                    index = index + 1;
                end
                if(obj.RX_FIFO1_full)
                    varargout{index} = 'RF1F';
                    index = index + 1;
                end
                if(obj.RX_FIFIO1_msg_lost)
                    varargout{index} = 'RF1ML';
                    index = index + 1;
                end
                if(obj.Highprt_msg)
                    varargout{index} = 'HPM';
                    index = index + 1;
                end
                if(obj.TX_complete)
                    varargout{index} = 'TC';
                    index = index + 1;
                end
                if(obj.TX_cancellation_finished)
                    varargout{index} = 'TCF';
                    index = index + 1;
                end
                if(obj.TXFIFIO_empty)
                    varargout{index} = 'TFE';
                    index = index + 1;
                end
                if(obj.TXEventFIFO_newentry)
                    varargout{index} = 'TEFN';
                    index = index + 1;
                end
                if(obj.TXEventFIFO_watermark)
                    varargout{index} = 'TEFW';
                    index = index + 1;
                end
                if(obj.TXEventFIFO_full)
                    varargout{index} = 'TEFF';
                    index = index + 1;
                end
                if(obj.TXEventFIFO_elemntlost)
                    varargout{index} = 'TEFL';
                    index = index + 1;
                end
                if(obj.Timestamp_wrparound)
                    varargout{index} = 'TSW';
                    index = index + 1;
                end
                if(obj.MsgRAM_accessfail)
                    varargout{index} = 'MRAF';
                    index = index + 1;
                end
                if(obj.Timeout_occured)
                    varargout{index} = 'TO';
                    index = index + 1;
                end
                if(obj.Msg_stored_dedrxbuf)
                    varargout{index} = 'DRB';
                    index = index + 1;
                end
                if(obj.Bit_error_corrected)
                    varargout{index} = 'BEC';
                    index = index + 1;
                end
                if(obj.Bit_error_uncorrected)
                    varargout{index} = 'BEU';
                    index = index + 1;
                end
                if(obj.Error_logging_overflow)
                    varargout{index} = 'ELO';
                    index = index + 1;
                end
                if(obj.Error_passive)
                    varargout{index} = 'EP';
                    index = index + 1;
                end
                if(obj.Warning_status)
                    varargout{index} = 'WS';
                    index = index + 1;
                end
                if(obj.Busoff_status)
                    varargout{index} = 'BOS';
                    index = index + 1;
                end
                if(obj.Watchdog_intr)
                    varargout{index} = 'WI';
                    index = index + 1;
                end
                if(obj.Protocol_err_arbtrphase)
                    varargout{index} = 'PEA';
                    index = index + 1;
                end
                if(obj.Protocol_err_dataphase)
                    varargout{index} = 'PED';
                    index = index + 1;
                end
                if(obj.Accs_reservedaddr)
                    varargout{index} = 'ARA';
                    index = index + 1;
                end
            end
        end
       
        
        function icon = getIconImpl(~)
            % Define a string as the icon for the System block in Simulink.
            icon = {'MCAN_INTERRUPT_STATUS'};
        end    
    end
    
    methods (Static, Access=protected)

        function header = getHeaderImpl()
            header = matlab.system.display.Header(...
                'Title','MCAN_INTERRUPT_STATUS',...
                'Text', sprintf('%s\n%s','Configure the CAN Interrupt Status block, to give status of selected interrupt. This block reads out values of MCAN_IR register'),...
                'ShowSourceLink', false);
        end

        function simMode = getSimulateUsingImpl(~)
            simMode = 'Interpreted execution';
        end
        
        function isVisible = showSimulateUsingImpl
            isVisible = false;
        end

        function groups = getPropertyGroupsImpl

            General = matlab.system.display.SectionGroup(...
             'Title', 'General', ...
             "PropertyList", {'MCAN_Instance', 'INT_STATUS_ALL', 'RX_FIFO0_newmsg', 'RX_FIFO0_wtrmark', 'RX_FIFO0_full', 'RX_FIFIO0_msg_lost', 'RX_FIFO1_newmsg', 'RX_FIFO1_wtrmark', 'RX_FIFO1_full', 'RX_FIFIO1_msg_lost', 'Highprt_msg', 'TX_complete', 'TX_cancellation_finished', 'TXFIFIO_empty', 'TXEventFIFO_newentry', 'TXEventFIFO_watermark', 'TXEventFIFO_full', 'TXEventFIFO_elemntlost', 'Timestamp_wrparound', 'MsgRAM_accessfail', 'Timeout_occured', 'Msg_stored_dedrxbuf', 'Bit_error_corrected', 'Bit_error_uncorrected', 'Error_logging_overflow', 'Error_passive', 'Warning_status', 'Busoff_status', 'Watchdog_intr', 'Protocol_err_arbtrphase', 'Protocol_err_dataphase', 'Accs_reservedaddr'});

           
            groups = [General];
        end
    end
    
    methods (Static)
        function name = getDescriptiveName()
            name = 'MCAN_INTERRUPT_STATUS';
        end
        
        function b = isSupportedContext(context)
            b = context.isCodeGenTarget('rtw');
        end
        
        function updateBuildInfo(buildInfo, context)
            if context.isCodeGenTarget('rtw')
                % Update buildInfo
                srcDir = fullfile(fileparts(mfilename('fullpath')),'src'); %#ok<NASGU>
                includeDir = fullfile(fileparts(mfilename('fullpath')),'include');
                addIncludePaths(buildInfo,includeDir);
                addIncludeFiles(buildInfo, 'MW_CAN.h', includeDir);
                addSourceFiles(buildInfo, 'MW_CAN.c', srcDir);
                % Use the following API's to add include files, sources and
                % linker flags
                %addIncludeFiles(buildInfo,'source.h',includeDir);
                %addSourceFiles(buildInfo,'source.c',srcDir);
                %addLinkFlags(buildInfo,{'-lSource'});
                %addLinkObjects(buildInfo,'sourcelib.a',srcDir);
                %addCompileFlags(buildInfo,{'-D_DEBUG=1'});
                %addDefines(buildInfo,'MY_DEFINE_1')
            end
        end
    end
end
