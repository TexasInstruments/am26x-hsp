importPackage(Packages.com.ti.debug.engine.scripting);
importPackage(Packages.com.ti.ccstudio.scripting.environment);
importPackage(Packages.java.lang);
importPackage(Packages.java.io);

ccxmlPath = arguments[0];
binary = arguments[1];

var script = ScriptingEnvironment.instance();
script.traceWrite("Begin scripting session");

script.traceWrite("setting config");

var debugServer = script.getServer("DebugServer.1");
debugServer.setConfig(ccxmlPath);

script.traceWrite("config set");

debugSession_r51 = debugServer.openSession(".*Cortex_R5_1*");
debugSession_r51.target.connect();
script.traceWrite("Connected");

debugSession_r51.memory.loadProgram(binary);
script.traceWrite("Loaded Helloworld");

//debugSession_r51.target.run();
debugSession_r51.target.runAsynch();
//debugSession_r51.target.halt();
script.traceWrite("Complete");

debugSession_r51.target.disconnect();
script.traceWrite("Disconnected");

debugSession_r51.terminate();

var dss  = 'C:/ti/ccs1250/ccs/ccs_base/scripting/bin';


