/*
 * $Revision $
 *
 * Abstract:
 *      TI Sitara interrupt handling.
 *
 
 */

#define S_FUNCTION_NAME HWI
#define S_FUNCTION_LEVEL 2

#include "simstruc.h"
#include "matrix.h"

/* Logical definitions */
#if (!defined(__cplusplus))
#  ifndef false
#   define false                       (0U)
#  endif
#  ifndef true
#   define true                        (1U)
#  endif
#endif

#define   NUMPARAMS             4
#define   INTERRUPT_TYPE       (ssGetSFcnParam(S,0))
#define   ISR_NUMBER           (ssGetSFcnParam(S,1))
#define   INSTANCE_NUMBER       (ssGetSFcnParam(S,2))
#define   SHOW_INPUTPORT       (ssGetSFcnParam(S,NUMPARAMS-1))
   

#ifndef MATLAB_MEX_FILE
/* Since we have a target file for this S-function, declare an error here
 * so that, if for some reason this file is being used (instead of the
 * target file) for code generation, we can trap this problem at compile
 * time. */
#  error This_file_can_be_used_only_during_simulation_inside_Simulink
#endif

/*====================*
 * S-function methods *
 *====================*/
#define MDL_CHECK_PARAMETERS
static void mdlCheckParameters(SimStruct *S)
{
    
    int_T isrNumber       = (int_T) (mxGetNumberOfElements(ISR_NUMBER));

    if ((isrNumber <0) && (isrNumber > 7)) {
        ssSetErrorStatus(S,"Interrupt Number must be an integer number.");
        return;
    }


    /* Check ISR_NUMBER */
    if (!mxIsDouble(ISR_NUMBER)) {
        ssSetErrorStatus(S,"Interrupt Number must be between 0 and 100'.");
        return;
    }

    if (!mxIsDouble(INSTANCE_NUMBER)) {
        ssSetErrorStatus(S,"Instance Number must be between 0 and 100'.");
        return;
    }

      
    if  (!mxIsChar(INTERRUPT_TYPE)) {
       ssSetErrorStatus(S,"Interrupt type must be a string'.");
    }

}

static void mdlInitializeSizes(SimStruct *S)
{
  
    
    ssSetNumSFcnParams(S, NUMPARAMS);
    if (ssGetNumSFcnParams(S) == ssGetSFcnParamsCount(S)) {
        mdlCheckParameters(S);
        if (ssGetErrorStatus(S) != NULL) {
            return;
        }
    } else {
        return; /* Parameter mismatch will be reported by Simulink */
    }


    ssSetSFcnParamNotTunable(        S, 0);
    ssSetSFcnParamNotTunable(        S, 1);
    ssSetSFcnParamNotTunable(        S, 2);
    ssSetSFcnParamNotTunable(        S, 3);
    

    if ((int_T)(mxGetPr(SHOW_INPUTPORT)[0])) {
        ssSetNumInputPorts(          S, 1);
        ssSetInputPortWidth(         S, 0, 1);
        ssSetInputPortDirectFeedThrough(S, 0, 1);
        ssSetInputPortDataType(      S, 0, SS_BOOLEAN);
    }
    else {
        ssSetNumInputPorts(          S, 0);
    }
    ssSetNumOutputPorts(             S, 1);
    ssSetOutputPortWidth(            S, 0, 1);
    /* All output elements are function-call, so we can set the data type of the
     * entire port to be function-call. */
    ssSetOutputPortDataType(         S, 0, SS_FCN_CALL);
    ssSetNumIWork(                   S, 0);
    ssSetNumRWork(                   S, 0);
    ssSetNumPWork(                   S, 0);
    ssSetNumSampleTimes(             S, 1);
    ssSetNumContStates(              S, 0);
    ssSetNumDiscStates(              S, 0);
    ssSetNumModes(                   S, 0);
    ssSetNumNonsampledZCs(           S, 0);
    ssSetOptions(                    S, (SS_OPTION_EXCEPTION_FREE_CODE |
                                         SS_OPTION_DISALLOW_CONSTANT_SAMPLE_TIME |
                                         SS_OPTION_ASYNCHRONOUS_INTERRUPT));


    /* Block has not internal states. Need change SimStateCompliance setting
     * if new state is added */
    ssSetSimStateCompliance(S,HAS_NO_SIM_STATE);

}


static void mdlInitializeSampleTimes(SimStruct *S)
{
    int_T i;

    ssSetSampleTime(S, 0, INHERITED_SAMPLE_TIME);
    ssSetOffsetTime(S, 0, FIXED_IN_MINOR_STEP_OFFSET);
    
    for(i = 0; i < ssGetOutputPortWidth(S,0); i++) {
        ssSetCallSystemOutput(S,i);
    }
}


static void mdlOutputs(SimStruct *S, int_T tid)
{
   
    
    InputPtrsType uPtrs = ssGetInputPortSignalPtrs(S,0);
    if (ssGetNumInputPorts(S) == 0) {
      
        ssCallSystemWithTid(S,0,tid);

    }
    else {
        InputBooleanPtrsType pU  = (InputBooleanPtrsType)uPtrs;
        if(*pU[0]) {
           ssCallSystemWithTid(S,0,tid);
        }
    }
                
}

static void mdlTerminate(SimStruct *S) {}

#define MDL_SET_INPUT_PORT_DATA_TYPE
static void mdlSetInputPortDataType(SimStruct *S, 
                                    int       port, 
                                    DTypeId   dataType)
{
    switch(dataType)
    {
        /* Accept Simulink data types */

      case SS_BOOLEAN:
        ssSetInputPortDataType(S, 0, dataType);
        break;
        
      default:
        /* Reject proposed data type */
        ssSetErrorStatus(S,"Invalid input port data type");
        break;
    }
} /* mdlSetInputPortDataType */

#define MDL_SET_DEFAULT_PORT_DATA_TYPES
static void mdlSetDefaultPortDataTypes(SimStruct *S)
{
    /* Set input port data type to bool */
    ssSetInputPortDataType(  S, 0, SS_BOOLEAN);            

} /* mdlSetDefaultPortDataTypes */

#define MDL_RTW
static void mdlRTW(SimStruct *S)
{

    char     str[256];
    mwSize   strLen=256;
    /* Write out parameters for this block.*/
    
    
    mxGetString(INTERRUPT_TYPE, &str[0], strLen);
      
    if (!ssWriteRTWParamSettings(S, 3,
                                    SSWRITE_VALUE_VECT,"ISR_NUMBER",
                                   (real_T *) mxGetPr(ISR_NUMBER),
                                    mxGetNumberOfElements(ISR_NUMBER),
                                    SSWRITE_VALUE_VECT,"INSTANCE_NUMBER",
                                    (real_T *)mxGetPr(INSTANCE_NUMBER),
                                    mxGetNumberOfElements(INSTANCE_NUMBER),
                                    SSWRITE_VALUE_STR,"INTERRUPT_TYPE",
                                    str

                                )){


        return; /* An error occurred which will be reported by SL */
    }
   
     
}

/*=============================*
 * Required S-function trailer *
 *=============================*/

#ifdef  MATLAB_MEX_FILE    /* Is this file being compiled as a MEX-file? */
#include "simulink.c"      /* MEX-file interface mechanism */
#else
#include "cg_sfun.h"       /* Code generation registration function */
#endif

/* EOF: HWI */
